#include <content/nodes.hpp>

#include <content/helpers/bmp.hpp>
#include <content/helpers/image.hpp>

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

    class NodeSaveBMP : public dp::Node {
    public:
        NodeSaveBMP() : Node("Save as BMP", {
            dp::Attribute(dp::Attribute::IOType::In,  dp::Attribute::Type::Buffer, "Image"),
            dp::Attribute(dp::Attribute::IOType::Out, dp::Attribute::Type::Buffer, "BMP")
        }) { }

        void process() override {
            std::string error;
            const auto image = decodeImage(this->getBufferOnInput(0), error);
            if (!image.has_value())
                this->throwNodeError(fmt::format("Couldn't decode the input as an image: {}", error));

            auto bmp = encodeBMP(*image);
            this->setBufferOnOutput(1, bmp);

            {
                const std::scoped_lock lock(m_pendingMutex);
                m_pendingBMP = std::move(bmp);
                m_hasPending = true;
            }
        }

        void reset() override {
            const std::scoped_lock lock(m_pendingMutex);
            m_pendingBMP.clear();
            m_hasPending = true;
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
                    // HINT(C4): this callback runs later, after the user picks a file. What does it need to carry with it?
                    [](const std::fs::path &path) {
                        wolv::io::File file(path, wolv::io::File::Mode::Create);
                        file.writeVector(m_bmp);
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

    void registerExportNodes() {
        ContentRegistry::DataProcessor::add<NodeSaveBMP>("Stegoooo", "Save as BMP");
    }

}
