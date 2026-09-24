#include <hex/plugin.hpp>
#include <hex/api/content_registry/data_processor.hpp>
#include <hex/data_processor/node.hpp>
#include <hex/data_processor/attribute.hpp>

using namespace hex;

class BufferCopyNode final : public dp::Node {
public:
    BufferCopyNode()
        : Node("Buffer copy", {
            dp::Attribute(dp::Attribute::IOType::In,  dp::Attribute::Type::Buffer, "Input"),
            dp::Attribute(dp::Attribute::IOType::Out, dp::Attribute::Type::Buffer, "Output")
        }) {}

    void process() override {
        const auto &input = getBufferOnInput(0);
        setBufferOnOutput(1, input);
    }
};

IMHEX_PLUGIN_SETUP("Buffer Node", "Your Name", "Buffer Data Processor example") {
    ContentRegistry::DataProcessor::add<BufferCopyNode>("Examples", "Buffer copy");
}
