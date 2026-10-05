#include <content/nodes.hpp>

#include <content/helpers/bmp.hpp>
#include <content/helpers/image.hpp>
#include <content/helpers/wav.hpp>

#include <hex/api/content_registry/data_processor.hpp>
#include <hex/data_processor/attribute.hpp>
#include <hex/data_processor/node.hpp>

#include <hex/helpers/fmt.hpp>
#include <hex/helpers/fs.hpp>
#include <hex/ui/imgui_imhex_extensions.h>

#include <wolv/io/file.hpp>

#include <imgui.h>

#include <mutex>
#include <string>
#include <vector>

namespace hex::plugin::stegoooo {

    // No outputs on purpose: ImHex only runs "end nodes" (inputs but no outputs) on its own,
    // anything with an output only runs when a node downstream reads from it
    class NodeSaveBMP : public dp::Node {
        public:
            NodeSaveBMP() : Node("Save as BMP", {
                    dp::Attribute(dp::Attribute::IOType::In,  dp::Attribute::Type::Buffer, "Image")
                    }) { }

            void process() override {
                std::string error;
                const auto image = decodeImage(this->getBufferOnInput(0), error);
                if (!image.has_value())
                    this->throwNodeError(fmt::format("Couldn't decode the input as an image: {}", error));

                auto bmp = encodeBMP(*image);

                {
                    const std::scoped_lock lock(m_pendingMutex);
                    m_pendingBMP = std::move(bmp);
                    m_hasPending = true;
                }
            }


        protected:
            void drawNode() override {
                {
                    const std::scoped_lock lock(m_pendingMutex);
                    if (m_hasPending) {
                        m_bmp = std::move(m_pendingBMP);
                        m_hasPending = false;
                    }
                }

                ImGuiExt::TextFormatted("BMP size: {} bytes", m_bmp.size());

                ImGui::BeginDisabled(m_bmp.empty());
                if (ImGui::Button("Save...")) {
                    hex::fs::openFileBrowser(
                            hex::fs::DialogMode::Save,
                            { { "Bitmap image", "bmp" } },
                    [bmp=m_bmp](const std::fs::path &path) {
                        wolv::io::File file(path, wolv::io::File::Mode::Create);
                        file.writeVector(bmp);
                    }
                );
            }
            ImGui::EndDisabled();
        }

    private:
        std::mutex m_pendingMutex;
        std::vector<u8> m_pendingBMP;
        bool m_hasPending = false;

        std::vector<u8> m_bmp;
    };

    // WAV bytes are saved as-is (no re-encoding), decodeWav() only validates them
    class NodeSaveWAV : public dp::Node {
        public:
            NodeSaveWAV() : Node("Save as WAV", {
                    dp::Attribute(dp::Attribute::IOType::In,  dp::Attribute::Type::Buffer, "Audio")
                    }) { }

            void process() override {
                std::string error;
                auto wav = decodeWav(this->getBufferOnInput(0), error);
                if (!wav.has_value())
                    this->throwNodeError(fmt::format("Couldn't decode the input as a WAV file: {}", error));

                auto format = fmt::format("{}-bit {}, {} ch, {} Hz, {:.2f} s",
                    wav->bitsPerSample, wav->formatTag == 3 ? "float" : "PCM", wav->channels, wav->sampleRate,
                    double(wav->frameCount()) / double(wav->sampleRate));

                {
                    const std::scoped_lock lock(m_pendingMutex);
                    m_pendingWAV = std::move(wav->bytes);
                    m_pendingFormat = std::move(format);
                    m_hasPending = true;
                }
            }


        protected:
            void drawNode() override {
                {
                    const std::scoped_lock lock(m_pendingMutex);
                    if (m_hasPending) {
                        m_wav = std::move(m_pendingWAV);
                        m_format = std::move(m_pendingFormat);
                        m_hasPending = false;
                    }
                }

                ImGuiExt::TextFormatted("Format: {}", m_format);
                ImGuiExt::TextFormatted("WAV size: {} bytes", m_wav.size());

                ImGui::BeginDisabled(m_wav.empty());
                if (ImGui::Button("Save...")) {
                    hex::fs::openFileBrowser(
                            hex::fs::DialogMode::Save,
                            { { "WAV audio", "wav" } },
                    [wav=m_wav](const std::fs::path &path) {
                        wolv::io::File file(path, wolv::io::File::Mode::Create);
                        file.writeVector(wav);
                    }
                );
            }
            ImGui::EndDisabled();
        }

    private:
        std::mutex m_pendingMutex;
        std::vector<u8> m_pendingWAV;
        std::string m_pendingFormat;
        bool m_hasPending = false;

        std::vector<u8> m_wav;
        std::string m_format;
    };

    void registerExportNodes() {
        ContentRegistry::DataProcessor::add<NodeSaveBMP>("Stegoooo", "Save as BMP");
        ContentRegistry::DataProcessor::add<NodeSaveWAV>("Stegoooo", "Save as WAV");
    }

}
