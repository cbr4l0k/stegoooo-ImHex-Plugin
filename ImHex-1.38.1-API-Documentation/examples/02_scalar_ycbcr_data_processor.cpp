#include <hex/plugin.hpp>
#include <hex/api/content_registry/data_processor.hpp>
#include <hex/data_processor/node.hpp>
#include <hex/data_processor/attribute.hpp>

#include <algorithm>

using namespace hex;

class RGBToYCbCrNode final : public dp::Node {
public:
    RGBToYCbCrNode()
        : Node("RGB -> YCbCr", {
            dp::Attribute(dp::Attribute::IOType::In,  dp::Attribute::Type::Integer, "R"),
            dp::Attribute(dp::Attribute::IOType::In,  dp::Attribute::Type::Integer, "G"),
            dp::Attribute(dp::Attribute::IOType::In,  dp::Attribute::Type::Integer, "B"),
            dp::Attribute(dp::Attribute::IOType::Out, dp::Attribute::Type::Integer, "Y"),
            dp::Attribute(dp::Attribute::IOType::Out, dp::Attribute::Type::Integer, "Cb"),
            dp::Attribute(dp::Attribute::IOType::Out, dp::Attribute::Type::Integer, "Cr")
        }) {}

    void process() override {
        const i128 r = getIntegerOnInput(0);
        const i128 g = getIntegerOnInput(1);
        const i128 b = getIntegerOnInput(2);

        const i128 y  = std::clamp<i128>(( 299*r + 587*g + 114*b) / 1000,       0, 255);
        const i128 cb = std::clamp<i128>((-169*r - 331*g + 500*b) / 1000 + 128, 0, 255);
        const i128 cr = std::clamp<i128>(( 500*r - 419*g -  81*b) / 1000 + 128, 0, 255);

        setIntegerOnOutput(3, y);
        setIntegerOnOutput(4, cb);
        setIntegerOnOutput(5, cr);
    }
};

IMHEX_PLUGIN_SETUP(
    "YCbCr Example",
    "Your Name",
    "Demonstrates the ImHex 1.38.1 Data Processor API"
) {
    ContentRegistry::DataProcessor::add<RGBToYCbCrNode>(
        "Images",
        "RGB -> YCbCr"
    );
}
