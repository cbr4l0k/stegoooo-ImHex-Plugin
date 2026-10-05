#include <content/nodes.hpp>

#include <content/helpers/image.hpp>
#include <content/helpers/stego.hpp>
#include <content/helpers/wav.hpp>

#include <hex/api/content_registry/data_processor.hpp>
#include <hex/data_processor/attribute.hpp>
#include <hex/data_processor/node.hpp>

#include <hex/helpers/fmt.hpp>
#include <hex/ui/imgui_imhex_extensions.h>

#include <cmath>
#include <mutex>
#include <string>
#include <utility>

namespace hex::plugin::stegoooo {

    // Base for nodes comparing an original and a modified cover, either both images or both WAV files
    class NodeCoverMetric : public dp::Node {
    public:
        using Node::Node;

    protected:
        // Decodes inputs 0 and 1 and calls `metric(original, modified)` with two Images or two Wavs
        auto compareCovers(auto &&metric) {
            const auto &originalData = this->getBufferOnInput(0);
            const auto &modifiedData = this->getBufferOnInput(1);

            std::string imageError, wavError;
            if (const auto original = decodeImage(originalData, imageError); original.has_value()) {
                const auto modified = decodeImage(modifiedData, imageError);
                if (!modified.has_value())
                    this->throwNodeError(fmt::format("Couldn't decode the modified image: {}", imageError));
                if (original->width != modified->width || original->height != modified->height)
                    this->throwNodeError("Images must have the same dimensions");

                return metric(*original, *modified);
            }

            if (const auto original = decodeWav(originalData, wavError); original.has_value()) {
                const auto modified = decodeWav(modifiedData, wavError);
                if (!modified.has_value())
                    this->throwNodeError(fmt::format("Couldn't decode the modified WAV: {}", wavError));
                if (original->formatTag != modified->formatTag || original->bitsPerSample != modified->bitsPerSample ||
                    original->channels != modified->channels || original->frameCount() != modified->frameCount())
                    this->throwNodeError("WAV files must have the same sample format, channel count and length");
                if (original->frameCount() == 0)
                    this->throwNodeError("WAV files contain no samples");

                return metric(*original, *modified);
            }

            this->throwNodeError(fmt::format("Original is neither a supported image ({}) nor a supported WAV ({})", imageError, wavError));
        }
    };

    class NodePSNR : public NodeCoverMetric {
    public:
        NodePSNR() : NodeCoverMetric("PSNR", {
            dp::Attribute(dp::Attribute::IOType::In,  dp::Attribute::Type::Buffer, "Original"),
            dp::Attribute(dp::Attribute::IOType::In,  dp::Attribute::Type::Buffer, "Modified"),
            dp::Attribute(dp::Attribute::IOType::Out, dp::Attribute::Type::Float,  "PSNR (dB)"),
            dp::Attribute(dp::Attribute::IOType::Out, dp::Attribute::Type::Float,  "MSE")
        }) { }

        void process() override {
            const auto [mseValue, psnrValue] = this->compareCovers([](const auto &original, const auto &modified) {
                return std::pair(mse(original, modified), psnr(original, modified));
            });

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

    class NodeSSIM : public NodeCoverMetric {
    public:
        NodeSSIM() : NodeCoverMetric("SSIM", {
            dp::Attribute(dp::Attribute::IOType::In,  dp::Attribute::Type::Buffer, "Original"),
            dp::Attribute(dp::Attribute::IOType::In,  dp::Attribute::Type::Buffer, "Modified"),
            dp::Attribute(dp::Attribute::IOType::Out, dp::Attribute::Type::Float, "SSIM")
        }) { }

        void process() override {
            const auto value = this->compareCovers([](const auto &original, const auto &modified) {
                return ssim(original, modified);
            });
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
