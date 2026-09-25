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
            RGBChannel::R,
            RGBChannel::G,
            RGBChannel::B
        };

        constexpr const char *channelName(RGBChannel channel) {
            switch (channel) {
                case RGBChannel::R: return "R (Red)";
                case RGBChannel::G: return "G (Green)";
                case RGBChannel::B: return "B (Blue)";
            }

            return "?";
        }

    }

    /**
     * @brief Splits an image into its three RGB channels and shows each one as its own picture
     */
    class NodeRGBChannels : public dp::Node {
    public:
        NodeRGBChannels() : Node("Split RGB channels", {
            dp::Attribute(dp::Attribute::IOType::In,  dp::Attribute::Type::Buffer,  "Image"),
            dp::Attribute(dp::Attribute::IOType::Out, dp::Attribute::Type::Buffer,  "R"),
            dp::Attribute(dp::Attribute::IOType::Out, dp::Attribute::Type::Buffer,  "G"),
            dp::Attribute(dp::Attribute::IOType::Out, dp::Attribute::Type::Buffer,  "B"),
            dp::Attribute(dp::Attribute::IOType::Out, dp::Attribute::Type::Integer, "Width"),
            dp::Attribute(dp::Attribute::IOType::Out, dp::Attribute::Type::Integer, "Height")
        }) { }

        // Runs on the Data Processor's worker thread, so it decodes but creates no textures
        void process() override {
            const auto &input = this->getBufferOnInput(0);

            std::string error;
            auto image = decodeImage(input, error);
            if (!image.has_value())
                this->throwNodeError(fmt::format("Couldn't decode the input as an image: {}", error));

            std::array<std::vector<u8>, 3> channels;
            for (size_t pixel = 0; pixel < image->pixelCount(); pixel += 1) {
                for (size_t channel = 0; channel < channels.size(); channel += 1)
                    channels[channel].push_back(image->rgba[pixel * 4 + channel]);
            }

            this->setBufferOnOutput(1, channels[0]);
            this->setBufferOnOutput(2, channels[1]);
            this->setBufferOnOutput(3, channels[2]);
            this->setIntegerOnOutput(4, image->width);
            this->setIntegerOnOutput(5, image->height);

            // Hand the image over to the render thread, which is the only place textures may be made
            {
                const std::scoped_lock lock(m_pendingMutex);
                m_pendingImage = std::move(*image);
                m_hasPendingImage = true;
            }
        }

        void reset() override {
            const std::scoped_lock lock(m_pendingMutex);

            m_pendingImage = { };
            m_hasPendingImage = true;
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
            this->collectPendingImage();
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

            if (!m_image.isValid()) {
                ImGuiExt::TextFormatted("Connect an image file to the input.");
                return;
            }

            ImGuiExt::TextFormatted("{} x {} pixels", m_image.width, m_image.height);

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
        constexpr static float ZoomFactor         = 3.0F;

        void collectPendingImage() {
            const std::scoped_lock lock(m_pendingMutex);

            if (!m_hasPendingImage)
                return;

            m_image = std::move(m_pendingImage);
            m_pendingImage = { };
            m_hasPendingImage = false;
            m_texturesDirty = true;
        }

        void refreshTextures() {
            if (!m_texturesDirty)
                return;

            m_texturesDirty = false;

            for (size_t i = 0; i < Channels.size(); i += 1) {
                m_textures[i].reset();

                const auto rgba = renderRGBChannel(m_image, Channels[i], m_preview);
                if (rgba.empty())
                    continue;

                auto texture = ImGuiExt::Texture::fromBitmap(
                    rgba.data(), rgba.size(),
                    int(m_image.width), int(m_image.height),
                    ImGuiExt::Texture::Filter::Nearest
                );

                m_textures[i] = std::move(texture);
            }
        }

        void drawChannel(RGBChannel channel, const ImGuiExt::Texture &texture) const {
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

            if (texture.isValid() && ImGui::IsItemHovered() && ImGui::IsKeyDown(ImGuiKey_LeftShift)) {
                ImGui::BeginTooltip();
                ImGui::TextUnformatted(channelName(channel));
                ImGui::Image(texture, ImVec2(size.x * ZoomFactor, size.y * ZoomFactor));
                ImGui::EndTooltip();
            }
        }

        // Written by process() on a worker thread, picked up by drawNode() on the render thread
        std::mutex m_pendingMutex;
        Image m_pendingImage;
        bool m_hasPendingImage = false;

        // Render thread only
        Image m_image;
        std::array<ImGuiExt::Texture, Channels.size()> m_textures;
        bool m_texturesDirty = false;

        ChannelPreview m_preview = ChannelPreview::Grayscale;
        int m_previewHeight = DefaultPreviewHeight;
    };

    void registerRGBNodes() {
        ContentRegistry::DataProcessor::add<NodeRGBChannels>("Stegoooo", "Split RGB channels");
    }

}
