#include <content/nodes.hpp>

#include <content/helpers/color_space.hpp>
#include <content/helpers/image.hpp>

#include <hex/api/content_registry/data_processor.hpp>
#include <hex/data_processor/attribute.hpp>
#include <hex/data_processor/node.hpp>

#include <hex/helpers/fmt.hpp>
#include <hex/helpers/scaling.hpp>
#include <hex/ui/imgui_imhex_extensions.h>

#include <imgui.h>
#include <nlohmann/json.hpp>

#include <array>
#include <mutex>
#include <string>

namespace hex::plugin::stegoooo {

    namespace {

        constexpr std::array Channels = {
            YCbCrChannel::Y,
            YCbCrChannel::Cb,
            YCbCrChannel::Cr
        };

        constexpr const char *channelName(YCbCrChannel channel) {
            switch (channel) {
                case YCbCrChannel::Y:  return "Y (Luma)";
                case YCbCrChannel::Cb: return "Cb (Blue-difference)";
                case YCbCrChannel::Cr: return "Cr (Red-difference)";
            }

            return "?";
        }

    }

    /**
     * @brief Splits an image into its three YCbCr channels and shows each one as its own picture
     *
     * The planes are also available on the outputs so they can be fed into other nodes.
     */
    class NodeYCbCrChannels : public dp::Node {
    public:
        NodeYCbCrChannels() : Node("Split YCbCr channels", {
            dp::Attribute(dp::Attribute::IOType::In,  dp::Attribute::Type::Buffer,  "Image"),
            dp::Attribute(dp::Attribute::IOType::Out, dp::Attribute::Type::Buffer,  "Y"),
            dp::Attribute(dp::Attribute::IOType::Out, dp::Attribute::Type::Buffer,  "Cb"),
            dp::Attribute(dp::Attribute::IOType::Out, dp::Attribute::Type::Buffer,  "Cr"),
            dp::Attribute(dp::Attribute::IOType::Out, dp::Attribute::Type::Integer, "Width"),
            dp::Attribute(dp::Attribute::IOType::Out, dp::Attribute::Type::Integer, "Height")
        }) { }

        // Runs on the Data Processor's worker thread, so it decodes and converts but creates no textures
        void process() override {
            const auto &input = this->getBufferOnInput(0);

            std::string error;
            const auto image = decodeImage(input, error);
            if (!image.has_value())
                this->throwNodeError(fmt::format("Couldn't decode the input as an image: {}", error));

            auto planes = toYCbCr(*image);

            this->setBufferOnOutput(1, planes.y);
            this->setBufferOnOutput(2, planes.cb);
            this->setBufferOnOutput(3, planes.cr);
            this->setIntegerOnOutput(4, planes.width);
            this->setIntegerOnOutput(5, planes.height);

            // Hand the planes over to the render thread, which is the only place textures may be made
            {
                const std::scoped_lock lock(m_pendingMutex);
                m_pendingPlanes = std::move(planes);
                m_hasPendingPlanes = true;
            }
        }


        void store(nlohmann::json &j) const override {
            j["preview"]       = u8(m_preview);
            j["previewHeight"] = m_previewHeight;
        }

        void load(const nlohmann::json &j) override {
            m_preview       = ChannelPreview(j.value("preview", u8(ChannelPreview::Grayscale)));
            m_previewHeight = j.value("previewHeight", DefaultPreviewHeight);
            m_texturesDirty = true;
        }

    protected:
        void drawNode() override {
            this->collectPendingPlanes();
            this->refreshTextures();

            ImGui::PushItemWidth(200_scaled);
            {
                int preview = int(m_preview);
                if (ImGui::Combo("Preview", &preview, "Grayscale\0Colorized\0")) {
                    m_preview = ChannelPreview(preview);
                    m_texturesDirty = true;
                }

                ImGui::SliderInt("Size", &m_previewHeight, MinPreviewHeight, MaxPreviewHeight, "%d px");
            }
            ImGui::PopItemWidth();

            if (!m_planes.isValid()) {
                ImGuiExt::TextFormatted("Connect an image file to the input.");
                return;
            }

            ImGuiExt::TextFormatted("{} x {} pixels", m_planes.width, m_planes.height);

            for (size_t i = 0; i < Channels.size(); i += 1) {
                if (i > 0)
                    ImGui::SameLine();

                this->drawChannel(Channels[i], m_textures[i]);
            }
        }

    private:
        constexpr static int DefaultPreviewHeight = 180;
        constexpr static int MinPreviewHeight     = 80;
        constexpr static int MaxPreviewHeight     = 400;
        constexpr static float ZoomFactor         = 5.0F;

        void collectPendingPlanes() {
            const std::scoped_lock lock(m_pendingMutex);

            if (!m_hasPendingPlanes)
                return;

            m_planes = std::move(m_pendingPlanes);
            m_pendingPlanes = { };
            m_hasPendingPlanes = false;
            m_texturesDirty = true;
        }

        void refreshTextures() {
            if (!m_texturesDirty)
                return;

            m_texturesDirty = false;

            for (size_t i = 0; i < Channels.size(); i += 1) {
                m_textures[i].reset();

                const auto rgba = renderChannel(m_planes, Channels[i], m_preview);
                if (rgba.empty())
                    continue;

                m_textures[i] = ImGuiExt::Texture::fromBitmap(
                    rgba.data(), rgba.size(),
                    int(m_planes.width), int(m_planes.height),
                    ImGuiExt::Texture::Filter::Nearest
                );
            }
        }

        void drawChannel(YCbCrChannel channel, const ImGuiExt::Texture &texture) const {
            const auto height = float(m_previewHeight);
            const auto size   = scaled(ImVec2(texture.getAspectRatio() * height, height));

            ImGui::BeginGroup();
            {
                ImGui::TextUnformatted(channelName(channel));

                if (texture.isValid())
                    ImGui::Image(texture, size);
                else
                    ImGui::Dummy(size);
            }
            ImGui::EndGroup();

            // Same trick the built-in visualizer nodes use: hold Shift while hovering for a bigger look
            if (texture.isValid() && ImGui::IsItemHovered() && ImGui::IsKeyDown(ImGuiKey_LeftShift)) {
                ImGui::BeginTooltip();
                ImGui::TextUnformatted(channelName(channel));
                ImGui::Image(texture, ImVec2(size.x * ZoomFactor, size.y * ZoomFactor));
                ImGui::EndTooltip();
            }
        }

        // Written by process() on a worker thread, picked up by drawNode() on the render thread
        std::mutex m_pendingMutex;
        YCbCrPlanes m_pendingPlanes;
        bool m_hasPendingPlanes = false;

        // Render thread only
        YCbCrPlanes m_planes;
        std::array<ImGuiExt::Texture, Channels.size()> m_textures;
        bool m_texturesDirty = false;

        ChannelPreview m_preview = ChannelPreview::Grayscale;
        int m_previewHeight = DefaultPreviewHeight;
    };

    void registerImageNodes() {
        ContentRegistry::DataProcessor::add<NodeYCbCrChannels>("Stegoooo", "Split YCbCr channels");
    }

}
