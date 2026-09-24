#include <hex/plugin.hpp>
#include <hex/api/events/events_provider.hpp>
#include <hex/providers/provider.hpp>
#include <hex/helpers/logger.hpp>

using namespace hex;

namespace {
    int eventToken;
}

IMHEX_PLUGIN_SETUP("Provider Event Example", "Your Name", "Event subscription example") {
    EventProviderOpened::subscribe(&eventToken, [](prv::Provider *provider) {
        if (provider != nullptr)
            log::info("Provider opened: {}", provider->getName());
    });
}
