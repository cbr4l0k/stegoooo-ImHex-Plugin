#include <hex/plugin.hpp>
#include <hex/api/imhex_api/provider.hpp>
#include <hex/helpers/logger.hpp>

using namespace hex;

IMHEX_PLUGIN_SETUP("Provider API Example", "Your Name", "Reads current provider metadata") {
    if (auto *provider = ImHexApi::Provider::get(); provider != nullptr) {
        log::info("Current provider: {} ({} bytes)", provider->getName(), provider->getActualSize());
    }
}
