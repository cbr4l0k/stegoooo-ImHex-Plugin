#include <content/nodes.hpp>

#include <content/helpers/image.hpp>
#include <content/helpers/stego.hpp>

#include <hex/api/content_registry/data_processor.hpp>
#include <hex/data_processor/attribute.hpp>
#include <hex/data_processor/node.hpp>

#include <hex/helpers/fmt.hpp>
#include <hex/ui/imgui_imhex_extensions.h>

#include <cmath>
#include <mutex>
#include <string>

namespace hex::plugin::stegoooo {

    class NodePSNR : public dp::Node {
    public:
        NodePSNR() : Node("PSNR", {
            dp::Attribute(dp::Attribute::IOType::In,  dp::Attribute::Type::Buffer, "Original"),
            dp::Attribute(dp::Attribute::IOType::In,  dp::Attribute::Type::Buffer, "Modified"),
            dp::Attribute(dp::Attribute::IOType::Out, dp::Attribute::Type::Float,  "PSNR (dB)"),
            dp::Attribute(dp::Attribute::IOType::Out, dp::Attribute::Type::Float,  "MSE")
        }) { }

        void process() override {
            std::string error;
            const auto original = decodeImage(this->getBufferOnInput(0), error);
            if (!original.has_value())
                this->throwNodeError(fmt::format("Couldn't decode the original image: {}", error));

            const auto modified = decodeImage(this->getBufferOnInput(1), error);
            if (!modified.has_value())
                this->throwNodeError(fmt::format("Couldn't decode the modified image: {}", error));

            if (original->width != modified->width || original->height != modified->height)
                this->throwNodeError("Images must have the same dimensions");
            const auto mseValue = mse(*original, *modified);
            const auto psnrValue = psnr(*original, *modified);

            this->setFloatOnOutput(2, psnrValue);
            this->setFloatOnOutput(3, mseValue);

            {
                const std::scoped_lock lock(m_pendingMutex);
                m_pendingPSNR = psnrValue;
                m_pendingMSE = mseValue;
                m_hasPending = true;
            }
        }


    protected:
        void drawNode() override {
            {
                const std::scoped_lock lock(m_pendingMutex);
                if (m_hasPending) {
                    m_psnr = m_pendingPSNR;
                    m_mse = m_pendingMSE;
                    m_hasPending = false;
                }
            }

            if (std::isinf(m_psnr))
                ImGuiExt::TextFormatted("PSNR: inf dB");
            else
                ImGuiExt::TextFormatted("PSNR: {:.4f} dB", m_psnr);
            ImGuiExt::TextFormatted("MSE: {:.4f}", m_mse);
        }

    private:
        std::mutex m_pendingMutex;
        double m_pendingPSNR = 0.0;
        double m_pendingMSE = 0.0;
        bool m_hasPending = false;

        double m_psnr = 0.0;
        double m_mse = 0.0;
    };

    class NodeSSIM : public dp::Node {
    public:
        NodeSSIM() : Node("SSIM", {
            dp::Attribute(dp::Attribute::IOType::In,  dp::Attribute::Type::Buffer, "Original"),
            dp::Attribute(dp::Attribute::IOType::In,  dp::Attribute::Type::Buffer, "Modified"),
            dp::Attribute(dp::Attribute::IOType::Out, dp::Attribute::Type::Float, "SSIM")
        }) { }

        void process() override {
            std::string error;
            const auto original = decodeImage(this->getBufferOnInput(0), error);
            if (!original.has_value())
                this->throwNodeError(fmt::format("Couldn't decode the original image: {}", error));

            const auto modified = decodeImage(this->getBufferOnInput(1), error);
            if (!modified.has_value())
                this->throwNodeError(fmt::format("Couldn't decode the modified image: {}", error));

            if (original->width != modified->width || original->height != modified->height)
                this->throwNodeError("Images must have the same dimensions");
            const auto value = ssim(*original, *modified);
            this->setFloatOnOutput(2, value);

            {
                const std::scoped_lock lock(m_pendingMutex);
                m_pendingValue = value;
                m_hasPending = true;
            }
        }


    protected:
        void drawNode() override {
            {
                const std::scoped_lock lock(m_pendingMutex);
                if (m_hasPending) {
                    m_value = m_pendingValue;
                    m_hasPending = false;
                }
            }

            ImGuiExt::TextFormatted("SSIM: {:.4f}", m_value);
        }

    private:
        std::mutex m_pendingMutex;
        double m_pendingValue = 0.0;
        bool m_hasPending = false;

        double m_value = 0.0;
    };

    void registerMetricNodes() {
        ContentRegistry::DataProcessor::add<NodePSNR>("Stegoooo", "PSNR");
        ContentRegistry::DataProcessor::add<NodeSSIM>("Stegoooo", "SSIM");
    }

}
