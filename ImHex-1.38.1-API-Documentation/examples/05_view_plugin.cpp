#include <hex/plugin.hpp>
#include <hex/api/content_registry/views.hpp>
#include <hex/ui/view.hpp>

#include <imgui.h>

using namespace hex;

class ExampleView final : public View::Window {
public:
    ExampleView() : View::Window("Example view", "") {}

    void drawContent() override {
        ImGui::TextUnformatted("Hello from an ImHex plugin.");
    }

    void drawHelpText() override {
        ImGui::TextUnformatted("Example help text.");
    }
};

IMHEX_PLUGIN_SETUP("View Example", "Your Name", "Registers a custom ImHex view") {
    ContentRegistry::Views::add<ExampleView>();
}
