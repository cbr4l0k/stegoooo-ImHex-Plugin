#include <hex/plugin.hpp>

#include <content/nodes.hpp>

using namespace hex::plugin::stegoooo;

IMHEX_PLUGIN_SETUP("Stegoooo", "cbr4l0k", "Steganography and image analysis blocks for the Data Processor") {
    registerImageNodes();
    registerRGBNodes();
    registerLSBNodes();
    registerWavLSBNodes();
    registerF5Nodes();
    registerMetricNodes();
    registerExportNodes();
    registerRandomNodes();
}
