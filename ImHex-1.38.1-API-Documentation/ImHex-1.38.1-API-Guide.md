# ImHex 1.38.1 C++ Plugin/API Guide

**Source basis:** the exact SDK header bundle you supplied (`imhex-1.38.1-sdk-headers.tar.gz`).  
**Coverage:** 95 public SDK header files under `lib/libimhex/include/hex`, plus the shipped CMake plugin macro.  
**Target version:** ImHex **1.38.1**. This guide intentionally does not substitute older ImHex API names when the 1.38.1 headers differ.

> This is a practical developer reference built from the SDK headers themselves. It documents what the headers expose; behavior that is only implemented in `.cpp` files is not claimed unless the header comments state it.

## Contents

1. [How to use this guide](#1-how-to-use-this-guide)
2. [API map](#2-api-map)
3. [Plugin entry points](#3-plugin-entry-points)
4. [Data Processor API](#4-data-processor-api)
5. [Content Registry](#5-content-registry)
6. [ImHexApi runtime services](#6-imhexapi-runtime-services)
7. [Events and requests](#7-events-and-requests)
8. [Providers](#8-providers)
9. [Views and UI](#9-views-and-ui)
10. [Managers](#10-managers)
11. [Localization](#11-localization)
12. [CMake/plugin build API](#12-cmakeplugin-build-api)
13. [Practical examples](#13-practical-examples)
14. [Debugging API-version mismatches](#14-debugging-api-version-mismatches)
15. [Complete header atlas](#15-complete-header-atlas)

---

## 1. How to use this guide

ImHex 1.38.1 does not expose its plugin API through one monolithic `content_registry.hpp`. The SDK is split by subsystem. For example, a Data Processor plugin needs:

```cpp
#include <hex/plugin.hpp>
#include <hex/api/content_registry/data_processor.hpp>
#include <hex/data_processor/node.hpp>
#include <hex/data_processor/attribute.hpp>
```

The important rule is: **include the subsystem header you actually use**. The SDK you supplied contains `hex/api/content_registry/` as a directory with 20 separate registry headers.

Useful local searches:

```bash
SDK=~/ImHex/sdk-install/usr/local/share/imhex/sdk/lib/libimhex/include

# Find a symbol
rg -n 'DataProcessor::|class Node|EVENT_DEF' "$SDK/hex"

# Find headers containing a class/function name
rg -l 'TaskManager' "$SDK/hex"

# List the registry surface
fd . "$SDK/hex/api/content_registry"
```

### Public surface vs. `impl`

Many headers expose a nested `impl` namespace alongside higher-level registration functions. As a practical compatibility rule, prefer the non-`impl` API when it exists. `impl` symbols are useful for understanding the SDK and sometimes are the only exposed hook, but their name signals a lower-level surface and they are more likely to change between versions.

---

## 2. API map

The SDK is organized around four ideas:

```text
plugin.hpp
   │
   ├── ContentRegistry::*   -> register new things in ImHex
   │      ├── DataProcessor
   │      ├── Views
   │      ├── UserInterface
   │      ├── Provider
   │      └── ...
   │
   ├── ImHexApi::*          -> interact with the running application
   │      ├── Provider
   │      ├── HexEditor
   │      ├── System
   │      └── ...
   │
   ├── EventManager/events  -> react to or request application events
   │
   └── Core abstractions
          ├── dp::Node / Attribute
          ├── prv::Provider
          ├── View
          └── managers/helpers
```

### Content Registry extension points

| Namespace | Header | Purpose |
|---|---|---|
| BackgroundServices | `background_services.hpp` | Register periodic/background service callbacks. |
| CommandPalette | `command_palette.hpp` | Commands, handlers, query results and custom command-palette content. |
| CommunicationInterface | `communication_interface.hpp` | Register JSON network endpoints. |
| DataFormatter | `data_formatter.hpp` | Export-menu formatters and Find-result exporters. |
| DataInformation | `data_information.hpp` | Register information-analysis sections. |
| DataInspector | `data_inspector.hpp` | Register Data Inspector value decoders/editors. |
| DataProcessor | `data_processor.hpp` | Register `dp::Node` types and separators. |
| Diffing | `diffing.hpp` | Register diff algorithms. |
| Disassemblers | `disassemblers.hpp` | Register disassembler architectures. |
| Experiments | `experiments.hpp` | Register/enable experimental features. |
| FileTypeHandler | `file_type_handler.hpp` | Associate file extensions with open/handling callbacks. |
| Hashes | `hashes.hpp` | Register hash implementations. |
| HexEditor | `hex_editor.hpp` | Register custom data/minimap visualizers. |
| PatternLanguage | `pattern_language.hpp` | Pattern-language functions, types, pragmas and visualizers. |
| Provider | `provider.hpp` | Register provider types. |
| Reports | `reports.hpp` | Register report generators. |
| Settings | `settings.hpp` | Settings UI widgets, read/write and change/save callbacks. |
| Tools | `tools.hpp` | Register tools. |
| UserInterface | `user_interface.hpp` | Menus, toolbar, sidebar, title-bar and welcome-screen extensions. |
| Views | `views.hpp` | Register custom `View` instances. |

### Runtime APIs

| Namespace | Header | Purpose |
|---|---|---|
| Bookmarks | `imhex_api/bookmarks.hpp` | Add/remove bookmarks. |
| Fonts | `imhex_api/fonts.hpp` | Register/use fonts and DPI conversion. |
| HexEditor | `imhex_api/hex_editor.hpp` | Selections, highlights, tooltips, hovering and virtual files. |
| Messaging | `imhex_api/messaging.hpp` | Register named byte-message handlers. |
| Provider | `imhex_api/provider.hpp` | Query, create, add, open, select and remove providers. |
| System | `imhex_api/system.hpp` | Application/window/system state, version/build info, tasks, update and restart/close. |

---

## 3. Plugin entry points

**Header:** `hex/plugin.hpp`

The normal plugin entry macro is:

```cpp
IMHEX_PLUGIN_SETUP(name, author, description) {
    // registration code
}
```

In 1.38.1, this macro generates the exported functions ImHex expects, including plugin name/author/description, compatible version, ImGui-context setup, feature/subcommand hooks, and `initializePlugin()`.

Minimal plugin:

```cpp
#include <hex/plugin.hpp>

IMHEX_PLUGIN_SETUP("My Plugin", "Me", "Example plugin") {
}
```

Other setup macros exposed by the header:

- `IMHEX_PLUGIN_SETUP_BUILTIN(...)` — built-in plugin form.
- `IMHEX_LIBRARY_SETUP(...)` — library-plugin initialization.
- `IMHEX_PLUGIN_FEATURES` / feature support macros — compile-time plugin features.

The generated compatibility function returns the compile-time `IMHEX_VERSION` value. That is one reason building against the matching SDK matters.

### Plugin-manager types

**Header:** `hex/api/plugin_manager.hpp`

Important types:

- `hex::Plugin` — loaded plugin instance and metadata.
- `hex::PluginManager` — load/unload/reload and plugin lookup.
- `hex::PluginFunctions` — function-pointer table used by the loader.
- `hex::SubCommand` — plugin command-line entry.
- `hex::Feature` — feature name + enabled flag.

For ordinary plugin authors, registration through `IMHEX_PLUGIN_SETUP` is the normal starting point; you usually do not construct `PluginFunctions` yourself.

---

## 4. Data Processor API

This is the subsystem most relevant to your YCbCr work.

### 4.1 `dp::Attribute`

**Header:** `hex/data_processor/attribute.hpp`

An attribute is a node port. It has two independent classifications:

```cpp
enum class Attribute::Type {
    Integer,
    Float,
    Buffer
};

enum class Attribute::IOType {
    In,
    Out
};
```

Constructor:

```cpp
Attribute(IOType ioType, Type type, UnlocalizedString unlocalizedName);
```
include/content/helpers/image.hpp

Useful accessors include `getId()`, `getIOType()`, `getType()`, `getUnlocalizedName()`, `getConnectedAttributes()`, `getOutputData()` and `getDefaultData()`.

A port therefore has a direction **and** a data type. This is why a Buffer connection cannot be wired directly to an Integer port in the Data Processor UI.

### 4.2 `dp::Node`

**Header:** `hex/data_processor/node.hpp`

Construction:

```cpp
Node(UnlocalizedString unlocalizedTitle, std::vector<Attribute> attributes);
```

The essential method is:

```cpp
virtual void process() = 0;
```

Optional hooks:

```cpp
virtual void reset();
virtual void store(nlohmann::json &j) const;
virtual void load(const nlohmann::json &j);
protected: virtual void drawNode();
```

Typed input access:

```cpp
const std::vector<u8>& getBufferOnInput(u32 index);
const i128&            getIntegerOnInput(u32 index);
const double&          getFloatOnInput(u32 index);
```

Typed output setters:

```cpp
void setBufferOnOutput(u32 index, std::span<const u8> data);
void setIntegerOnOutput(u32 index, i128 integer);
void setFloatOnOutput(u32 index, double floatingPoint);
```

Other useful node facilities:

- `throwNodeError(message)` — report a node error.
- `setOverlayData(address, data)` — write overlay data associated with the node's current overlay.
- `Node::interrupt()` — interrupt processing.
- `setAttributes(...)` — replace the attribute vector from a derived class.
- `resetOutputData()` / `resetProcessedInputs()` — processing-state helpers.
- `setPosition()` / `getPosition()` — graph position.
- `store()` / `load()` — persist node-specific state.

### 4.3 Attribute indices

A node receives an ordered `std::vector<Attribute>` in its constructor, and the typed `get...OnInput(index)` / `set...OnOutput(index)` methods use an attribute index. Keep the constructor order explicit and stable.

Example:

```cpp
Node("Example", {
    Attribute(In,  Integer, "A"),   // 0
    Attribute(In,  Integer, "B"),   // 1
    Attribute(Out, Integer, "Sum")  // 2
})
```

Then `getIntegerOnInput(0)`, `getIntegerOnInput(1)` and `setIntegerOnOutput(2, ...)` correspond to those positions.

### 4.4 Registering a node — **the correct 1.38.1 API**

**Header:** `hex/api/content_registry/data_processor.hpp`

The registry namespace in your SDK is:

```cpp
hex::ContentRegistry::DataProcessor
```

—not `ContentRegistry::DataProcessorNode`.

Registration template:

```cpp
template<std::derived_from<dp::Node> T, typename... Args>
void add(
    const UnlocalizedString &unlocalizedCategory,
    const UnlocalizedString &unlocalizedName,
    Args&&... args
);
```

Example:

```cpp
ContentRegistry::DataProcessor::add<MyNode>(
    "Images",
    "My node"
);
```

A separator can be registered with:

```cpp
ContentRegistry::DataProcessor::addSeparator();
```

### 4.5 Complete scalar RGB → YCbCr node

This example uses only APIs that are present in the supplied 1.38.1 headers:

```cpp
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

IMHEX_PLUGIN_SETUP("YCbCr Example", "Your Name", "Data Processor example") {
    ContentRegistry::DataProcessor::add<RGBToYCbCrNode>(
        "Images", "RGB -> YCbCr"
    );
}
```

A copy of this example is included as `examples/02_scalar_ycbcr_data_processor.cpp`.

### 4.6 Buffer nodes

For image/file processing, Buffer ports are the relevant type:

```cpp
const auto &input = getBufferOnInput(0);
setBufferOnOutput(1, input);
```

`setBufferOnOutput` takes `std::span<const u8>`, so contiguous byte containers can be passed without inventing integer ports for every byte.

Important distinction: the Data Processor API gives you bytes; it does **not** itself imply that an encoded PNG/JPEG buffer has already been decoded into RGB pixels. Decoding is a separate operation or library concern.

---

## 5. Content Registry

The Content Registry is the main plugin-extension surface: your plugin registers new nodes, views, providers, menu entries, tools, settings, visualizers, etc.

### Registry overview

| Registry | Header | What it extends |
|---|---|---|
| BackgroundServices | `background_services.hpp` | Register periodic/background service callbacks. |
| CommandPalette | `command_palette.hpp` | Commands, handlers, query results and custom command-palette content. |
| CommunicationInterface | `communication_interface.hpp` | Register JSON network endpoints. |
| DataFormatter | `data_formatter.hpp` | Export-menu formatters and Find-result exporters. |
| DataInformation | `data_information.hpp` | Register information-analysis sections. |
| DataInspector | `data_inspector.hpp` | Register Data Inspector value decoders/editors. |
| DataProcessor | `data_processor.hpp` | Register `dp::Node` types and separators. |
| Diffing | `diffing.hpp` | Register diff algorithms. |
| Disassemblers | `disassemblers.hpp` | Register disassembler architectures. |
| Experiments | `experiments.hpp` | Register/enable experimental features. |
| FileTypeHandler | `file_type_handler.hpp` | Associate file extensions with open/handling callbacks. |
| Hashes | `hashes.hpp` | Register hash implementations. |
| HexEditor | `hex_editor.hpp` | Register custom data/minimap visualizers. |
| PatternLanguage | `pattern_language.hpp` | Pattern-language functions, types, pragmas and visualizers. |
| Provider | `provider.hpp` | Register provider types. |
| Reports | `reports.hpp` | Register report generators. |
| Settings | `settings.hpp` | Settings UI widgets, read/write and change/save callbacks. |
| Tools | `tools.hpp` | Register tools. |
| UserInterface | `user_interface.hpp` | Menus, toolbar, sidebar, title-bar and welcome-screen extensions. |
| Views | `views.hpp` | Register custom `View` instances. |

### 5.1 Views

**Header:** `hex/api/content_registry/views.hpp`

```cpp
template<std::derived_from<View> T, typename... Args>
void ContentRegistry::Views::add(Args&&... args);

View* ContentRegistry::Views::getViewByName(const UnlocalizedString&);
View* ContentRegistry::Views::getFocusedView();
```

There is also `setFullScreenView<T>(...)` for a full-screen view.

### 5.2 User interface registration

**Header:** `hex/api/content_registry/user_interface.hpp`

The registry can extend:

- top-level main menus,
- menu items and separators,
- submenus,
- welcome-screen entries,
- footer items,
- toolbar items,
- sidebar items,
- title-bar buttons,
- welcome-screen quick settings toggles.

Menu entries accept callbacks for execution, enabled state and selected state, and can be associated with a `View` and `Shortcut`.

### 5.3 Settings

**Header:** `hex/api/content_registry/settings.hpp`

Built-in setting widget classes exposed by the SDK:

- `Checkbox`
- `SliderInteger`
- `SliderFloat`
- `SliderDataSize`
- `ColorPicker`
- `DropDown`
- `TextBox`
- `FilePicker`
- `Label`

The header also exposes `SettingsValue`, templated read/write helpers, `onChange(...)`, and `onSave(...)` callbacks.

### 5.4 Pattern Language

**Header:** `hex/api/content_registry/pattern_language.hpp`

Plugins can register:

- pragmas,
- functions,
- dangerous functions,
- types,
- visualizers,
- inline visualizers.

The registry also exposes the shared `pl::PatternLanguage` runtime and a runtime lock.

### 5.5 Providers

**Header:** `hex/api/content_registry/provider.hpp`

```cpp
template<std::derived_from<prv::Provider> T>
void ContentRegistry::Provider::add(bool addToList = true);
```

Provider registration is distinct from `ImHexApi::Provider`, which manages provider **instances** at runtime.

### 5.6 Hex-editor visualizers

**Header:** `hex/api/content_registry/hex_editor.hpp`

Custom visualizers derive from `ContentRegistry::HexEditor::DataVisualizer`. The registry provides `addDataVisualizer<T>(...)`, lookup by name, and minimap visualizer registration.

---

## 6. ImHexApi runtime services

`ContentRegistry` adds capabilities. `ImHexApi` generally interacts with the **currently running ImHex state**.

| API | Header | Purpose |
|---|---|---|
| Bookmarks | `imhex_api/bookmarks.hpp` | Add/remove bookmarks. |
| Fonts | `imhex_api/fonts.hpp` | Register/use fonts and DPI conversion. |
| HexEditor | `imhex_api/hex_editor.hpp` | Selections, highlights, tooltips, hovering and virtual files. |
| Messaging | `imhex_api/messaging.hpp` | Register named byte-message handlers. |
| Provider | `imhex_api/provider.hpp` | Query, create, add, open, select and remove providers. |
| System | `imhex_api/system.hpp` | Application/window/system state, version/build info, tasks, update and restart/close. |

### 6.1 Current provider

**Header:** `hex/api/imhex_api/provider.hpp`

Common operations:

```cpp
prv::Provider *ImHexApi::Provider::get();
std::vector<prv::Provider*> ImHexApi::Provider::getProviders();
void ImHexApi::Provider::setCurrentProvider(i64 index);
i64 ImHexApi::Provider::getCurrentProviderIndex();
bool ImHexApi::Provider::isValid();
```

Provider lifecycle helpers include `add(...)`, `remove(...)`, `createProvider(...)` and `openProvider(...)`.

### 6.2 Hex Editor

**Header:** `hex/api/imhex_api/hex_editor.hpp`

The API can:

- get/set/clear the current selection,
- create static background/foreground highlights,
- register dynamic highlighting providers,
- add/remove tooltips,
- register hover-highlighting providers,
- add virtual files,
- query the hovered region.

`ProviderRegion` combines a `Region` with the associated `prv::Provider*`.

### 6.3 Bookmarks

**Header:** `hex/api/imhex_api/bookmarks.hpp`

```cpp
u64 add(u64 address, size_t size, const std::string &name,
        const std::string &comment, color_t color = 0x00000000);

u64 add(Region region, const std::string &name,
        const std::string &comment, color_t color = 0x00000000);

void remove(u64 id);
```

### 6.4 System API

**Header:** `hex/api/imhex_api/system.hpp`

Notable groups:

- application lifecycle: `closeImHex`, `restartImHex`;
- taskbar progress and target FPS;
- display/global/native scaling;
- theme detection;
- OS/GPU/OpenGL information;
- ImHex version, commit, branch and build time;
- update checks/update execution;
- startup tasks and migration routines;
- frame-rate unlock and post-processing shader.

For version checks, `getImHexVersion()` is exposed directly by this API.

---

## 7. Events and requests

**Core header:** `hex/api/event_manager.hpp`

The SDK defines typed events using `EVENT_DEF` / `EVENT_DEF_NO_LOG`. Those macros generate event types with static convenience methods:

```cpp
EventType::subscribe(callback);
EventType::subscribe(token, callback);
EventType::unsubscribe(...);
EventType::post(...);
```

The lower-level manager also provides templated `EventManager::subscribe<E>()`, `unsubscribe<E>()` and `post<E>()`.

### Token-based subscription

A stable pointer can be used as a token:

```cpp
namespace { int token; }

EventProviderOpened::subscribe(&token, [](prv::Provider *provider) {
    // ...
});

// Later:
EventProviderOpened::unsubscribe(&token);
```

### Built-in event/request catalog

#### `events_gui.hpp`

| Event/request | Parameters | NO_LOG |
|---|---|---|
| EventViewOpened | View* | no |
| EventViewClosed | View* | no |
| EventDPIChanged | float, float | no |
| EventWindowFocused | bool | no |
| EventWindowClosing | GLFWwindow* | no |
| EventWindowDeinitializing | GLFWwindow* | no |
| EventOSThemeChanged | — | no |
| EventFrameBegin | — | yes |
| EventFrameEnd | — | yes |
| EventSetTaskBarIconState | u32, u32, u32 | yes |
| EventImGuiElementRendered | ImGuiID, const std::array<float, 4>& | yes |

#### `events_interaction.hpp`

| Event/request | Parameters | NO_LOG |
|---|---|---|
| EventFileLoaded | std::fs::path | no |
| EventDataChanged | prv::Provider * | no |
| EventHighlightingChanged | — | no |
| EventRegionSelected | ImHexApi::HexEditor::ProviderRegion | no |
| EventThemeChanged | — | no |
| EventBookmarkCreated | ImHexApi::Bookmarks::Entry& | no |
| EventPatchCreated | const u8*, u64, const PatchKind | no |
| EventPatternEvaluating | — | no |
| EventPatternExecuted | const std::string& | no |
| EventPatternEditorChanged | const std::string& | no |
| EventStoreContentDownloaded | const std::fs::path& | no |
| EventStoreContentRemoved | const std::fs::path& | no |
| EventAchievementUnlocked | const Achievement& | no |
| EventSearchBoxClicked | u32 | no |
| EventFileDragged | bool | no |
| EventFileDropped | std::fs::path | no |

#### `events_lifecycle.hpp`

| Event/request | Parameters | NO_LOG |
|---|---|---|
| EventImHexStartupFinished | — | no |
| EventCloseButtonPressed | — | no |
| EventImHexClosing | — | no |
| EventFirstLaunch | — | no |
| EventAnySettingChanged | — | no |
| EventAbnormalTermination | int | no |
| EventImHexUpdated | SemanticVersion, SemanticVersion | no |
| EventCrashRecovered | const std::exception & | no |
| EventProjectOpened | — | no |
| EventProjectSaved | — | no |
| EventNativeMessageReceived | std::vector<u8> | no |
| EventRegisterImGuiTests | ImGuiTestEngine* | no |

#### `events_provider.hpp`

| Event/request | Parameters | NO_LOG |
|---|---|---|
| EventProviderCreated | std::shared_ptr<prv::Provider> | no |
| EventProviderOpened | prv::Provider * | no |
| EventProviderChanged | prv::Provider *, prv::Provider * | no |
| EventProviderSaved | prv::Provider * | no |
| EventProviderClosing | prv::Provider *, bool * | no |
| EventProviderClosed | prv::Provider * | no |
| EventProviderDeleted | prv::Provider * | no |
| EventProviderDirtied | prv::Provider * | no |
| EventProviderDataInserted | prv::Provider *, u64, u64 | no |
| EventProviderDataModified | prv::Provider *, u64, u64, const u8* | no |
| EventProviderDataRemoved | prv::Provider *, u64, u64 | no |

#### `requests_gui.hpp`

| Event/request | Parameters | NO_LOG |
|---|---|---|
| RequestOpenWindow | std::string | no |
| RequestUpdateWindowTitle | — | no |
| RequestChangeTheme | std::string | no |
| RequestOpenPopup | std::string | no |
| RequestSetPostProcessingShader | std::string, std::string | no |

#### `requests_interaction.hpp`

| Event/request | Parameters | NO_LOG |
|---|---|---|
| RequestHexEditorSelectionChange | ImHexApi::HexEditor::ProviderRegion | no |
| RequestPatternEditorSelectionChange | u32, u32 | no |
| RequestJumpToPattern | const pl::ptrn::Pattern* | no |
| RequestAddBookmark | Region, std::string, std::string, color_t, u64* | no |
| RequestRemoveBookmark | u64 | no |
| RequestSetPatternLanguageCode | std::string | no |
| RequestTriggerPatternEvaluation | — | no |
| RequestOpenFile | std::fs::path | no |
| RequestAddVirtualFile | std::fs::path, std::vector<u8>, Region | no |
| RequestOpenCommandPalette | — | no |

#### `requests_lifecycle.hpp`

| Event/request | Parameters | NO_LOG |
|---|---|---|
| RequestAddInitTask | std::string, bool, std::function<bool()> | no |
| RequestAddExitTask | std::string, std::function<bool()> | no |
| RequestCloseImHex | bool | no |
| RequestRestartImHex | — | no |
| RequestInitThemeHandlers | — | no |
| SendMessageToMainInstance | const std::string, const std::vector<u8>& | no |

#### `requests_provider.hpp`

| Event/request | Parameters | NO_LOG |
|---|---|---|
| RequestCreateProvider | std::string, bool, bool, std::shared_ptr<hex::prv::Provider> * | no |
| RequestOpenProvider | std::shared_ptr<prv::Provider> | no |
| MovePerProviderData | prv::Provider *, prv::Provider * | no |

---

## 8. Providers

### 8.1 Provider base class

**Header:** `hex/providers/provider.hpp`

`hex::prv::Provider` is the abstract data-source interface. A custom provider must implement the fundamental lifecycle/capability/data methods, including:

```cpp
virtual OpenResult open() = 0;
virtual void close() = 0;
virtual bool isAvailable() const = 0;
virtual bool isReadable() const = 0;
virtual bool isWritable() const = 0;
virtual bool isResizable() const = 0;
virtual bool isSavable() const = 0;

virtual void readRaw(u64 offset, void *buffer, size_t size) = 0;
virtual void writeRaw(u64 offset, const void *buffer, size_t size) = 0;

virtual u64 getActualSize() const = 0;
virtual UnlocalizedString getTypeName() const = 0;
virtual std::string getName() const = 0;
virtual const char *getIcon() const = 0;
```

The base class supplies higher-level operations around those primitives, including patched/overlay-aware `read`/`write`, resize/insert/remove, save/save-as, overlays, paging, base address, undo/redo, settings serialization and dirty-state tracking.

### 8.2 Provider registration vs provider instances

Two APIs have intentionally different jobs:

```text
ContentRegistry::Provider  -> register a provider TYPE
ImHexApi::Provider         -> manage provider INSTANCES
```

That distinction is useful throughout the SDK: registries extend what ImHex *can create*, while `ImHexApi` talks to the current application state.

### 8.3 Other provider headers

The SDK also exposes buffering, overlays, in-memory/cached providers and undo/redo support under `hex/providers/`.

---

## 9. Views and UI

### 9.1 `hex::View`

**Header:** `hex/ui/view.hpp`

A view is the base abstraction for plugin windows. Important hooks:

```cpp
virtual void draw(ImGuiWindowFlags extraFlags = ImGuiWindowFlags_None) = 0;
virtual void drawContent() = 0;
virtual void drawAlwaysVisibleContent();
virtual bool shouldDraw() const;
virtual bool shouldProcess() const;
virtual bool hasViewMenuItemEntry() const;
```

Lifecycle hooks:

```cpp
protected:
virtual void onOpen();
virtual void onClose();
```

View forms exposed by the header:

- `View::Window` — normal dockable window; implement `drawContent()` and `drawHelpText()`.
- `View::Special` — plugin handles special window drawing.
- `View::Floating` — floating/non-dockable window.
- `View::Scrolling` — window with scrolling behavior.
- `View::Modal` — modal window.
- `View::FullScreen` — full-screen view.

Register a view with:

```cpp
ContentRegistry::Views::add<MyView>(constructorArgs...);
```

### 9.2 Shortcuts

**Header:** `hex/api/shortcut_manager.hpp`

Shortcuts are composed from `Key` objects using `+`, for example the header explicitly describes forms such as:

```cpp
CTRL + ALT + Keys::A
```

`ShortcutManager` supports global shortcuts and view-specific shortcuts, enabled callbacks, programmatic execution, pause/resume, lookup and update.

---

## 10. Managers

### TaskManager

**Header:** `hex/api/task_manager.hpp`

Three task modes are exposed:

- `createTask(...)`
- `createBackgroundTask(...)`
- `createBlockingTask(...)`

A `Task` supports progress updates, interruption, exception state, max value and an interrupt callback. `TaskHolder` is the external handle used to query or interrupt a running task.

`TaskManager::doLater(...)` and `doLaterOnce(...)` schedule work for deferred execution; `runWhenTasksFinished(...)` registers a completion action.

### ThemeManager

**Header:** `hex/api/theme_manager.hpp`

Loads/applies themes, registers color/style handlers, exports the current theme and manages accent color.

### ProjectFile

**Header:** `hex/api/project_file_manager.hpp`

Plugins can register project-file handlers and per-provider handlers, each with a base path, callback and `required` flag.

### LayoutManager

**Header:** `hex/api/layout_manager.hpp`

Save/load layouts to files or strings, lock layouts, close views, and register load/store callbacks.

### WorkspaceManager

**Header:** `hex/api/workspace_manager.hpp`

Create/switch/import/export/remove workspaces and query current/all workspace definitions.

### TutorialManager / AchievementManager

The SDK also exposes APIs to create interactive tutorials/help and register achievements with dependencies/progress.

---

## 11. Localization

**Header:** `hex/api/localization_manager.hpp`

Three commonly encountered types are:

- `UnlocalizedString` — stores an untranslated key/string and accepts `const char*` or `std::string`.
- `Lang` — resolves/localizes a string dynamically.
- `LangConst` — compile-time hashed language key, produced by the `_lang` literal.

This explains why most registry APIs take `UnlocalizedString` rather than plain display strings: the same registration surface can use localization keys.

For experimental/local plugins, a plain string literal is accepted by `UnlocalizedString`, which is why examples such as:

```cpp
ContentRegistry::DataProcessor::add<MyNode>("Examples", "My node");
```

are valid at the type level.

---

## 12. CMake/plugin build API

**SDK file:** `cmake/modules/ImHexPlugin.cmake`

The central macro is:

```cmake
add_imhex_plugin(...)
```

Arguments parsed by the 1.38.1 macro:

```text
Option:
  LIBRARY_PLUGIN

Single-value:
  NAME
  IMHEX_VERSION

Multi-value:
  SOURCES
  INCLUDES
  LIBRARIES
  FEATURES
```

For a normal external plugin, the macro creates a module library with suffix `.hexplug`. Library plugins use `.hexpluglib`. The macro sets the target to **C++23**, links `libimhex` plus ImHex dependencies, defines `IMHEX_PROJECT_NAME`, `IMHEX_VERSION` and `IMHEX_PLUGIN_NAME`, and configures plugin output/rpath/resource handling.

The bootstrap that locates/loads this macro is supplied by the external plugin build/template environment; the exact macro above is what the SDK itself guarantees in the supplied archive.

---

## 13. Practical examples

The documentation package contains compilable-style source examples under `examples/`:

1. `01_minimal_plugin.cpp` — plugin entry point only.
2. `02_scalar_ycbcr_data_processor.cpp` — exact 1.38.1 Data Processor registration and integer ports.
3. `03_buffer_node.cpp` — Buffer input/output node.
4. `04_event_subscription.cpp` — token-based event subscription.
5. `05_view_plugin.cpp` — register a custom `View::Window`.
6. `06_current_provider.cpp` — query the current provider through `ImHexApi`.

### The key fix to our earlier YCbCr attempt

For **your** SDK the correct include/registration pair is:

```cpp
#include <hex/api/content_registry/data_processor.hpp>

ContentRegistry::DataProcessor::add<MyNode>(...);
```

There is no `hex/api/content_registry.hpp` file in the supplied 1.38.1 SDK, and the registry namespace is not `ContentRegistry::DataProcessorNode`.

---

## 14. Debugging API-version mismatches

When an example from the internet fails to compile, verify it against your SDK instead of assuming the example is current.

### Missing header

```bash
fd content_registry ~/ImHex/sdk-install/usr/local/share/imhex/sdk/lib/libimhex/include/hex
```

### Find the current registration namespace

```bash
rg -n 'namespace ContentRegistry::DataProcessor|void add\('   ~/ImHex/sdk-install/usr/local/share/imhex/sdk/lib/libimhex/include/hex/api/content_registry
```

### Find a class and all call sites available in headers

```bash
rg -n 'class TaskManager|TaskManager::' "$SDK/hex"
```

### Check plugin build compatibility

The plugin setup macro exposes the compile-time `IMHEX_VERSION` as the plugin's compatible version. Use the SDK matching the ImHex binary you intend to load the plugin into.

---

## 15. Complete header atlas

This is an index of every SDK header in the supplied bundle. The “notable types/events” column is mechanically extracted and is intended for discovery, not as a replacement for the curated sections above.

| Header | Purpose | Notable types/events |
|---|---|---|
| `hex/api/achievement_manager.hpp` | Achievement registration, progress and dependencies. | AchievementManager, Achievement, AchievementNode |
| `hex/api/content_registry/background_services.hpp` | Content Registry extension point for background services. | — |
| `hex/api/content_registry/command_palette.hpp` | Content Registry extension point for command palette. | Type, QueryResult, Entry, Handler, ContentDisplay |
| `hex/api/content_registry/communication_interface.hpp` | Content Registry extension point for communication interface. | — |
| `hex/api/content_registry/data_formatter.hpp` | Content Registry extension point for data formatter. | ExportMenuEntry, FindOccurrence, DecodeType, FindExporterEntry |
| `hex/api/content_registry/data_information.hpp` | Content Registry extension point for data information. | InformationSection |
| `hex/api/content_registry/data_inspector.hpp` | Content Registry extension point for data inspector. | NumberDisplayStyle, Entry |
| `hex/api/content_registry/data_processor.hpp` | Content Registry extension point for data processor. | Entry |
| `hex/api/content_registry/diffing.hpp` | Content Registry extension point for diffing. | DifferenceType, Algorithm |
| `hex/api/content_registry/disassemblers.hpp` | Content Registry extension point for disassemblers. | Instruction, Architecture |
| `hex/api/content_registry/experiments.hpp` | Content Registry extension point for experiments. | Experiment |
| `hex/api/content_registry/file_type_handler.hpp` | Content Registry extension point for file type handler. | Entry |
| `hex/api/content_registry/hashes.hpp` | Content Registry extension point for hashes. | Hash, Function |
| `hex/api/content_registry/hex_editor.hpp` | Content Registry extension point for hex editor. | DataVisualizer, MiniMapVisualizer |
| `hex/api/content_registry/pattern_language.hpp` | Content Registry extension point for pattern language. | FunctionDefinition, TypeDefinition, Visualizer |
| `hex/api/content_registry/provider.hpp` | Content Registry extension point for provider. | Entry |
| `hex/api/content_registry/reports.hpp` | Content Registry extension point for reports. | ReportGenerator |
| `hex/api/content_registry/settings.hpp` | Content Registry extension point for settings. | Widget, Interface, Checkbox, SliderInteger, SliderFloat, SliderDataSize, … |
| `hex/api/content_registry/tools.hpp` | Content Registry extension point for tools. | Entry |
| `hex/api/content_registry/user_interface.hpp` | Content Registry extension point for user interface. | Icon, MainMenuItem, MenuItem, SidebarItem, TitleBarButton, WelcomeScreenQuickSettingsToggle |
| `hex/api/content_registry/views.hpp` | Content Registry extension point for views. | — |
| `hex/api/event_manager.hpp` | Typed event bus, subscription, unsubscription and posting. | event_name, EventId, EventBase, Event, EventManager |
| `hex/api/events/events_gui.hpp` | Built-in event type declarations. | GLFWwindow, EventViewOpened, EventViewClosed, EventDPIChanged, EventWindowFocused, EventWindowClosing, … |
| `hex/api/events/events_interaction.hpp` | Built-in event type declarations. | EventFileLoaded, EventDataChanged, EventHighlightingChanged, EventRegionSelected, EventThemeChanged, EventBookmarkCreated, … |
| `hex/api/events/events_lifecycle.hpp` | Built-in event type declarations. | ImGuiTestEngine, EventImHexStartupFinished, EventCloseButtonPressed, EventImHexClosing, EventFirstLaunch, EventAnySettingChanged, … |
| `hex/api/events/events_provider.hpp` | Built-in event type declarations. | Provider, EventProviderCreated, EventProviderOpened, EventProviderChanged, EventProviderSaved, EventProviderClosing, … |
| `hex/api/events/requests_gui.hpp` | Built-in request/message event declarations. | RequestOpenWindow, RequestUpdateWindowTitle, RequestChangeTheme, RequestOpenPopup, RequestSetPostProcessingShader |
| `hex/api/events/requests_interaction.hpp` | Built-in request/message event declarations. | RequestHexEditorSelectionChange, RequestPatternEditorSelectionChange, RequestJumpToPattern, RequestAddBookmark, RequestRemoveBookmark, RequestSetPatternLanguageCode, … |
| `hex/api/events/requests_lifecycle.hpp` | Built-in request/message event declarations. | RequestAddInitTask, RequestAddExitTask, RequestCloseImHex, RequestRestartImHex, RequestInitThemeHandlers, SendMessageToMainInstance |
| `hex/api/events/requests_provider.hpp` | Built-in request/message event declarations. | RequestCreateProvider, RequestOpenProvider, MovePerProviderData |
| `hex/api/imhex_api/bookmarks.hpp` | Runtime `ImHexApi` functions for bookmarks. | Entry |
| `hex/api/imhex_api/fonts.hpp` | Runtime `ImHexApi` functions for fonts. | ImFont, Offset, MergeFont, Font, FontDefinition |
| `hex/api/imhex_api/hex_editor.hpp` | Runtime `ImHexApi` functions for hex editor. | Highlighting, Tooltip, ProviderRegion |
| `hex/api/imhex_api/messaging.hpp` | Runtime `ImHexApi` functions for messaging. | — |
| `hex/api/imhex_api/provider.hpp` | Runtime `ImHexApi` functions for provider. | — |
| `hex/api/imhex_api/system.hpp` | Runtime `ImHexApi` functions for system. | ImVec2, ImFontAtlas, GLFWwindow, AutoResetBase, ProgramArguments, InitialWindowProperties, … |
| `hex/api/layout_manager.hpp` | Persist, load and manage ImGui layouts. | ImGuiTextBuffer, LayoutManager, Layout |
| `hex/api/localization_manager.hpp` | Localization, `UnlocalizedString`, `Lang`, languages. | UnlocalizedString, PathEntry, LanguageDefinition, LangConst, Lang, std |
| `hex/api/plugin_manager.hpp` | Runtime plugin objects, metadata and plugin loading/management. | ImGuiContext, SubCommand, Type, Feature, PluginFunctions, Plugin, … |
| `hex/api/project_file_manager.hpp` | Project-file load/store handlers, including per-provider state. | Provider, ProjectFile, Handler, ProviderHandler |
| `hex/api/shortcut_manager.hpp` | Global/view shortcuts and key combinations. | ImGuiWindow, KeyEquivalent, View, Key, Shortcut, ShortcutManager, … |
| `hex/api/task_manager.hpp` | Foreground, background and blocking tasks with progress/interruption. | TaskHolder, TaskManager, Task, TaskInterruptor |
| `hex/api/theme_manager.hpp` | Theme loading, color/style handlers and accent color. | ThemeManager, Style, ThemeHandler, StyleHandler |
| `hex/api/tutorial_manager.hpp` | Interactive tutorials and contextual help. | TutorialManager, Position, Tutorial, Step, Highlight, Message |
| `hex/api/workspace_manager.hpp` | Workspace create/switch/import/export management. | WorkspaceManager, Workspace |
| `hex/api_urls.hpp` | SDK header. | — |
| `hex/data_processor/attribute.hpp` | Typed Data Processor input/output ports. | Node, Attribute, Type, IOType |
| `hex/data_processor/link.hpp` | Connections between Data Processor attributes. | Link |
| `hex/data_processor/node.hpp` | Base class for Data Processor nodes. | Provider, Overlay, Node, NodeError |
| `hex/helpers/auto_reset.hpp` | Helper utilities for auto reset. | AutoResetBase, AutoReset |
| `hex/helpers/binary_pattern.hpp` | Binary-pattern helper. | BinaryPattern, Pattern |
| `hex/helpers/concepts.hpp` | Helper utilities for concepts. | always_false, ICloneable |
| `hex/helpers/crypto.hpp` | Cryptographic helper functions. | Provider, AESMode, KeyLength |
| `hex/helpers/debugging.hpp` | Helper utilities for debugging. | StackTraceResult |
| `hex/helpers/default_paths.hpp` | ImHex default/search paths. | DefaultPath, ConfigPath, DataPath, PluginPath |
| `hex/helpers/encoding_file.hpp` | Encoding-file support. | EncodingFile, Type |
| `hex/helpers/fmt.hpp` | Helper utilities for fmt. | — |
| `hex/helpers/fs.hpp` | Filesystem helpers. | DialogMode, ItemFilter |
| `hex/helpers/http_requests.hpp` | HTTP request abstraction. | HttpRequest, ResultBase, Result |
| `hex/helpers/http_requests_emscripten.hpp` | Helper utilities for http requests emscripten. | — |
| `hex/helpers/http_requests_native.hpp` | Helper utilities for http requests native. | — |
| `hex/helpers/keys.hpp` | Helper utilities for keys. | Keys |
| `hex/helpers/literals.hpp` | Helper utilities for literals. | — |
| `hex/helpers/logger.hpp` | Logging API. | LogEntry |
| `hex/helpers/magic.hpp` | Magic/signature helpers. | Provider, FoundPattern |
| `hex/helpers/menu_items.hpp` | Helper utilities for menu items. | — |
| `hex/helpers/opengl.hpp` | OpenGL helpers and texture/shader abstractions. | Vector, Matrix, RotationSequence, MatrixElements, Shader, BufferType, … |
| `hex/helpers/patches.hpp` | Patch-related helpers. | Provider, IPSError, PatchKind, Patches |
| `hex/helpers/scaling.hpp` | Helper utilities for scaling. | — |
| `hex/helpers/semantic_version.hpp` | Semantic-version representation. | SemanticVersion |
| `hex/helpers/tar.hpp` | TAR archive helper. | mtar_t, Tar, Mode |
| `hex/helpers/types.hpp` | Core helper types. | Region, NonNull |
| `hex/helpers/udp_server.hpp` | UDP server helper. | UDPServer |
| `hex/helpers/utils.hpp` | Helper utilities for utils. | Provider, SizeTypeImpl |
| `hex/helpers/utils_linux.hpp` | Helper utilities for utils linux. | — |
| `hex/helpers/utils_macos.hpp` | Helper utilities for utils macos. | GLFWwindow |
| `hex/plugin.hpp` | Plugin entry-point macros (`IMHEX_PLUGIN_SETUP`, library setup, features). | PluginFunctionHelperInstantiation, PluginFeatureFunctionHelper, PluginSubCommandsFunctionHelper |
| `hex/providers/buffered_reader.hpp` | Buffered reading helper for providers. | ProviderReader |
| `hex/providers/cached_provider.hpp` | Provider caching wrapper/base. | CachedProvider, Block |
| `hex/providers/memory_provider.hpp` | In-memory provider implementation. | MemoryProvider |
| `hex/providers/overlay.hpp` | Provider overlays. | Overlay |
| `hex/providers/provider.hpp` | Abstract data-provider interface and common provider behavior. | IProviderLoadInterface, IProviderSidebarInterface, IProviderFilePicker, IProviderMenuItems, MenuEntry, IProviderDataDescription, … |
| `hex/providers/provider_data.hpp` | Per-provider data storage helpers. | Provider, PerProvider |
| `hex/providers/undo_redo/operations/operation.hpp` | Undo/redo API. | Provider, Operation |
| `hex/providers/undo_redo/operations/operation_group.hpp` | Undo/redo API. | OperationGroup |
| `hex/providers/undo_redo/stack.hpp` | Undo/redo API. | Provider, Stack |
| `hex/subcommands/subcommands.hpp` | Command-line subcommand support. | — |
| `hex/test/test_provider.hpp` | Testing support exposed by the SDK. | TestProvider |
| `hex/test/tests.hpp` | Testing support exposed by the SDK. | Test, Tests, TestSequence, TestSequenceExecutor, ImGuiTestSequence, ImGuiTestSequenceExecutor |
| `hex/ui/banner.hpp` | Banner UI helper. | BannerBase, Banner |
| `hex/ui/imgui_imhex_extensions.h` | ImHex-specific ImGui extensions. | ImGuiCustomCol, ImGuiCustomStyle, Texture, Filter, ImHexCustomData, Styles, … |
| `hex/ui/popup.hpp` | Popup helpers. | PopupBase, Popup |
| `hex/ui/toast.hpp` | Toast notifications. | ToastBase, Toast |
| `hex/ui/view.hpp` | View/window abstraction used for plugin UI. | View, Window, Special, Floating, Scrolling, Modal, … |
| `hex/ui/widgets.hpp` | Reusable ImHex UI widgets. | SearchableWidget |

---

## Source-of-truth note

This guide was generated from the headers in your supplied **ImHex 1.38.1 SDK bundle**. If a future ImHex version disagrees with this document, the headers for that version are authoritative. The CSV files included with this documentation are intended to make version-to-version diffing easier.
