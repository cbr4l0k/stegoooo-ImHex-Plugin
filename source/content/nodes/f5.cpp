#include <content/nodes.hpp>

#include <content/helpers/bmp.hpp>
#include <content/helpers/f5.hpp>
#include <content/helpers/image.hpp>

#include <hex/api/content_registry/data_processor.hpp>
#include <hex/data_processor/attribute.hpp>
#include <hex/data_processor/node.hpp>

#include <hex/helpers/fmt.hpp>
#include <hex/helpers/scaling.hpp>
#include <hex/ui/imgui_imhex_extensions.h>

#include <imgui.h>
#include <nlohmann/json.hpp>

#include <algorithm>
#include <mutex>
#include <string>

namespace hex::plugin::stegoooo {

    class NodeF5Embed : public dp::Node {
    public:
        NodeF5Embed() : Node("F5 hide", {
            dp::Attribute(dp::Attribute::IOType::In,  dp::Attribute::Type::Buffer,  "Cover image"),
            dp::Attribute(dp::Attribute::IOType::In,  dp::Attribute::Type::Buffer,  "Message"),
            dp::Attribute(dp::Attribute::IOType::Out, dp::Attribute::Type::Buffer,  "Stego image (BMP)"),
            dp::Attribute(dp::Attribute::IOType::Out, dp::Attribute::Type::Integer, "Capacity (bytes)")
        }) { }

        void process() override {
            const auto &input   = this->getBufferOnInput(0);
            const auto &message = this->getBufferOnInput(1);

            F5Settings settings;
            {
                const std::scoped_lock lock(m_settingsMutex);
                settings = m_settings;
            }

            std::string error;
            const auto image = decodeImage(input, error);
            if (!image.has_value())
                this->throwNodeError(fmt::format("Couldn't decode the cover image: {}", error));

            const auto capacity = f5Capacity(*image, settings);
            F5Stats stats;
            const auto stego = f5Embed(*image, message, settings, stats, error);
            if (!stego.has_value())
                this->throwNodeError(error);

            this->setBufferOnOutput(2, encodeBMP(*stego));
            this->setIntegerOnOutput(3, capacity);

            {
                const std::scoped_lock lock(m_pendingMutex);
                m_pendingStats = stats;
                m_hasPending = true;
            }
        }

        void store(nlohmann::json &j) const override {
            const std::scoped_lock lock(m_settingsMutex);
            j["quality"]  = m_settings.quality;
            j["password"] = m_settings.password;
            j["channels"] = u8(m_settings.channels);
        }

        void load(const nlohmann::json &j) override {
            const std::scoped_lock lock(m_settingsMutex);
            // value() only falls back when a key is missing, a key of the wrong type throws
            try {
                m_settings.quality  = std::clamp(j.value("quality", 75), 1, 90);
                m_settings.password = j.value("password", std::string());
                m_settings.channels = F5Channels(std::clamp(j.value("channels", int(F5Channels::All)), 0, 1));
            } catch (const nlohmann::json::exception &) {
                m_settings = { };
            }
        }

    protected:
        void drawNode() override {
            this->drawSettings();

            {
                const std::scoped_lock lock(m_pendingMutex);
                if (m_hasPending) {
                    m_stats = m_pendingStats;
                    m_hasPending = false;
                }
            }

            ImGuiExt::TextFormatted("k: {}", m_stats.k);
            ImGuiExt::TextFormatted("Message: {} bits", m_stats.messageBits);
            ImGuiExt::TextFormatted("Changes: {}", m_stats.changes);
            ImGuiExt::TextFormatted("Shrinkage: {}", m_stats.shrinkage);
            ImGuiExt::TextFormatted("Repaired blocks: {}", m_stats.repairedBlocks);
            ImGuiExt::TextFormatted("Squash margin: {}", m_stats.squashMargin);
            const auto efficiency = m_stats.changes == 0 ? 0.0 : double(m_stats.messageBits) / double(m_stats.changes);
            ImGuiExt::TextFormatted("Embedding efficiency: {:.2f} bits/change", efficiency);
        }

    private:
        void drawSettings() {
            const std::scoped_lock lock(m_settingsMutex);
            ImGui::PushItemWidth(200_scaled);
            {
                ImGui::SliderInt("Quality", &m_settings.quality, 1, 90);
                ImGui::InputText("Password", m_settings.password);

                int channels = int(m_settings.channels);
                if (ImGui::Combo("Channels", &channels, "Y only\0Y, Cb and Cr\0"))
                    m_settings.channels = F5Channels(channels);
            }
            ImGui::PopItemWidth();
        }

        mutable std::mutex m_settingsMutex;
        F5Settings m_settings;

        std::mutex m_pendingMutex;
        F5Stats m_pendingStats;
        bool m_hasPending = false;

        F5Stats m_stats;
    };

    class NodeF5Extract : public dp::Node {
    public:
        NodeF5Extract() : Node("F5 retrieve", {
            dp::Attribute(dp::Attribute::IOType::In,  dp::Attribute::Type::Buffer, "Stego image"),
            dp::Attribute(dp::Attribute::IOType::Out, dp::Attribute::Type::Buffer, "Message")
        }) { }

        void process() override {
            const auto &input = this->getBufferOnInput(0);

            F5Settings settings;
            {
                const std::scoped_lock lock(m_settingsMutex);
                settings = m_settings;
            }

            std::string error;
            const auto image = decodeImage(input, error);
            if (!image.has_value())
                this->throwNodeError(fmt::format("Couldn't decode the stego image: {}", error));

            auto message = f5Extract(*image, settings, error);
            if (!message.has_value())
                this->throwNodeError(error);

            this->setBufferOnOutput(1, *message);

            std::string preview;
            preview.reserve(std::min<size_t>(64, message->size()));
            for (const auto value : std::span(*message).first(std::min<size_t>(64, message->size())))
                preview.push_back(value >= 0x20 && value <= 0x7E ? char(value) : '.');

            {
                const std::scoped_lock lock(m_pendingMutex);
                m_pendingLength = message->size();
                m_pendingPreview = std::move(preview);
                m_hasPending = true;
            }
        }

        void store(nlohmann::json &j) const override {
            const std::scoped_lock lock(m_settingsMutex);
            j["quality"]  = m_settings.quality;
            j["password"] = m_settings.password;
            j["channels"] = u8(m_settings.channels);
        }

        void load(const nlohmann::json &j) override {
            const std::scoped_lock lock(m_settingsMutex);
            // value() only falls back when a key is missing, a key of the wrong type throws
            try {
                m_settings.quality  = std::clamp(j.value("quality", 75), 1, 90);
                m_settings.password = j.value("password", std::string());
                m_settings.channels = F5Channels(std::clamp(j.value("channels", int(F5Channels::All)), 0, 1));
            } catch (const nlohmann::json::exception &) {
                m_settings = { };
            }
        }

    protected:
        void drawNode() override {
            this->drawSettings();

            {
                const std::scoped_lock lock(m_pendingMutex);
                if (m_hasPending) {
                    m_length = m_pendingLength;
                    m_preview = std::move(m_pendingPreview);
                    m_hasPending = false;
                }
            }

            ImGuiExt::TextFormatted("Extracted: {} bytes", m_length);
            ImGuiExt::TextFormatted("Preview: {}", m_preview);
        }

    private:
        void drawSettings() {
            const std::scoped_lock lock(m_settingsMutex);
            ImGui::PushItemWidth(200_scaled);
            {
                ImGui::SliderInt("Quality", &m_settings.quality, 1, 90);
                ImGui::InputText("Password", m_settings.password);

                int channels = int(m_settings.channels);
                if (ImGui::Combo("Channels", &channels, "Y only\0Y, Cb and Cr\0"))
                    m_settings.channels = F5Channels(channels);
            }
            ImGui::PopItemWidth();
        }

        mutable std::mutex m_settingsMutex;
        F5Settings m_settings;

        std::mutex m_pendingMutex;
        size_t m_pendingLength = 0;
        std::string m_pendingPreview;
        bool m_hasPending = false;

        size_t m_length = 0;
        std::string m_preview;
    };

    class NodeJPEGQuantize : public dp::Node {
    public:
        NodeJPEGQuantize() : Node("JPEG quantize", {
            dp::Attribute(dp::Attribute::IOType::In,  dp::Attribute::Type::Buffer, "Image"),
            dp::Attribute(dp::Attribute::IOType::Out, dp::Attribute::Type::Buffer, "Quantized image (BMP)")
        }) { }

        void process() override {
            const auto &input = this->getBufferOnInput(0);

            int quality;
            {
                const std::scoped_lock lock(m_settingsMutex);
                quality = m_quality;
            }

            std::string error;
            const auto image = decodeImage(input, error);
            if (!image.has_value())
                this->throwNodeError(fmt::format("Couldn't decode the input as an image: {}", error));

            const auto quantized = jpegQuantize(*image, quality, error);
            if (!quantized.has_value())
                this->throwNodeError(error);

            this->setBufferOnOutput(1, encodeBMP(*quantized));
        }

        void store(nlohmann::json &j) const override {
            const std::scoped_lock lock(m_settingsMutex);
            j["quality"] = m_quality;
        }

        void load(const nlohmann::json &j) override {
            const std::scoped_lock lock(m_settingsMutex);
            try {
                m_quality = std::clamp(j.value("quality", 75), 1, 90);
            } catch (const nlohmann::json::exception &) {
                m_quality = 75;
            }
        }

    protected:
        void drawNode() override {
            const std::scoped_lock lock(m_settingsMutex);
            ImGui::PushItemWidth(200_scaled);
            ImGui::SliderInt("Quality", &m_quality, 1, 90);
            ImGui::PopItemWidth();
        }

    private:
        mutable std::mutex m_settingsMutex;
        int m_quality = 75;
    };

    void registerF5Nodes() {
        ContentRegistry::DataProcessor::add<NodeF5Embed>("Stegoooo", "F5 hide");
        ContentRegistry::DataProcessor::add<NodeF5Extract>("Stegoooo", "F5 retrieve");
        ContentRegistry::DataProcessor::add<NodeJPEGQuantize>("Stegoooo", "JPEG quantize");
    }

}
