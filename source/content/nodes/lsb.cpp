#include <content/nodes.hpp>

#include <content/helpers/bmp.hpp>
#include <content/helpers/image.hpp>
#include <content/helpers/stego.hpp>

#include <hex/api/content_registry/data_processor.hpp>
#include <hex/data_processor/attribute.hpp>
#include <hex/data_processor/node.hpp>

#include <hex/helpers/fmt.hpp>
#include <hex/ui/imgui_imhex_extensions.h>

#include <algorithm>
#include <mutex>
#include <string>

namespace hex::plugin::stegoooo {

    class NodeLSBEmbed : public dp::Node {
    public:
        NodeLSBEmbed() : Node("LSB hide", {
            dp::Attribute(dp::Attribute::IOType::In,  dp::Attribute::Type::Buffer,  "Cover image"),
            dp::Attribute(dp::Attribute::IOType::In,  dp::Attribute::Type::Buffer,  "Message"),
            dp::Attribute(dp::Attribute::IOType::Out, dp::Attribute::Type::Buffer,  "Stego image (BMP)"),
            dp::Attribute(dp::Attribute::IOType::Out, dp::Attribute::Type::Integer, "Capacity (bytes)")
        }) { }

        void process() override {
            const auto &input   = this->getBufferOnInput(0);
            const auto &message = this->getBufferOnInput(1);

            std::string error;
            const auto image = decodeImage(input, error);
            if (!image.has_value())
                this->throwNodeError(fmt::format("Couldn't decode the cover image: {}", error));

            const auto capacity = lsbCapacity(*image);
            const auto stego = lsbEmbed(*image, message, error);
            if (!stego.has_value())
                this->throwNodeError(error);

            this->setBufferOnOutput(2, encodeBMP(*stego));
            this->setIntegerOnOutput(3, capacity);

            {
                const std::scoped_lock lock(m_pendingMutex);
                m_pendingCapacity = capacity;
                m_pendingUsed = message.size();
                m_hasPending = true;
            }
        }


    protected:
        void drawNode() override {
            {
                const std::scoped_lock lock(m_pendingMutex);
                if (m_hasPending) {
                    m_capacity = m_pendingCapacity;
                    m_used = m_pendingUsed;
                    m_hasPending = false;
                }
            }

            const auto percent = m_capacity == 0 ? 0.0 : 100.0 * double(m_used) / double(m_capacity);
            ImGuiExt::TextFormatted("Capacity: {} bytes", m_capacity);
            ImGuiExt::TextFormatted("Used: {} bytes ({:.1f}%)", m_used, percent);
        }

    private:
        std::mutex m_pendingMutex;
        size_t m_pendingCapacity = 0;
        size_t m_pendingUsed = 0;
        bool m_hasPending = false;

        size_t m_capacity = 0;
        size_t m_used = 0;
    };

    class NodeLSBExtract : public dp::Node {
    public:
        NodeLSBExtract() : Node("LSB retrieve", {
            dp::Attribute(dp::Attribute::IOType::In,  dp::Attribute::Type::Buffer, "Stego image"),
            dp::Attribute(dp::Attribute::IOType::Out, dp::Attribute::Type::Buffer, "Message")
        }) { }

        void process() override {
            const auto &input = this->getBufferOnInput(0);

            std::string error;
            const auto image = decodeImage(input, error);
            if (!image.has_value())
                this->throwNodeError(fmt::format("Couldn't decode the stego image: {}", error));

            auto message = lsbExtract(*image, error);
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


    protected:
        void drawNode() override {
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
        std::mutex m_pendingMutex;
        size_t m_pendingLength = 0;
        std::string m_pendingPreview;
        bool m_hasPending = false;

        size_t m_length = 0;
        std::string m_preview;
    };

    void registerLSBNodes() {
        ContentRegistry::DataProcessor::add<NodeLSBEmbed>("Stegoooo", "LSB hide");
        ContentRegistry::DataProcessor::add<NodeLSBExtract>("Stegoooo", "LSB retrieve");
    }

}
