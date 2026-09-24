# ImHex 1.38.1 Mechanical Declaration Index

Generated from the supplied SDK headers. This is a discovery aid, not a curated stability guarantee. It includes declarations from public headers, including some `impl` and private/internal class members.

## `libimhex/include/hex/api/achievement_manager.hpp`

Types: `class AchievementManager`, `class Achievement`, `struct AchievementNode`

Function-like declarations:
- `std::function<void(Achievement &)> m_clickCallback;`
- `AchievementManager() = delete;`
- `auto newAchievement = std::make_unique<T>(std::forward<decltype(args)>(args)...);`
- `auto &achievement = addAchievement<T>(std::forward<decltype(args)>(args)...);`
- `static void unlockAchievement(const UnlocalizedString &unlocalizedCategory, const UnlocalizedString &unlocalizedName);`
- `static const std::unordered_map<UnlocalizedString, std::unordered_map<UnlocalizedString, std::unique_ptr<Achievement>>>& getAchievements();`
- `static const std::unordered_map<UnlocalizedString, std::vector<AchievementNode*>>& getAchievementStartNodes(bool rebuild = true);`
- `static const std::unordered_map<UnlocalizedString, std::list<AchievementNode>>& getAchievementNodes(bool rebuild = true);`
- `static void loadProgress();`
- `static void storeProgress();`
- `static void clearTemporary();`
- `static std::pair<u32, u32> getProgress();`
- `static void achievementAdded();`
- `static Achievement& addAchievementImpl(std::unique_ptr<Achievement> &&newAchievement);`

## `libimhex/include/hex/api/content_registry/background_services.hpp`
Namespaces: `ContentRegistry::BackgroundServices`, `impl`

Function-like declarations:
- `void stopServices();`
- `void registerService(const UnlocalizedString &unlocalizedString, const impl::Callback &callback);`

## `libimhex/include/hex/api/content_registry/command_palette.hpp`
Namespaces: `ContentRegistry::CommandPalette`, `impl`

Types: `enum class Type`, `struct QueryResult`, `struct Entry`, `struct Handler`, `struct ContentDisplay`

Function-like declarations:
- `const std::vector<Entry>& getEntries();`
- `const std::vector<Handler>& getHandlers();`
- `std::optional<ContentDisplay>& getDisplayedContent();`
- `void add( Type type, const std::string &command, const UnlocalizedString &unlocalizedDescription, const impl::DisplayCallback &displayCallback, const impl::ExecuteCallback &executeCallback = [](auto) { return std::nullopt;`
- `void addHandler( Type type, const std::string &command, const impl::QueryCallback &queryCallback, const impl::DisplayCallback &displayCallback);`
- `void setDisplayedContent(const impl::ContentDisplayCallback &displayCallback);`
- `void openWithContent(const impl::ContentDisplayCallback &displayCallback);`

## `libimhex/include/hex/api/content_registry/communication_interface.hpp`
Namespaces: `ContentRegistry::CommunicationInterface`, `impl`

Function-like declarations:
- `const std::map<std::string, NetworkCallback>& getNetworkEndpoints();`
- `void registerNetworkEndpoint(const std::string &endpoint, const impl::NetworkCallback &callback);`

## `libimhex/include/hex/api/content_registry/data_formatter.hpp`
Namespaces: `prv`, `ContentRegistry::DataFormatter`, `impl`

Types: `struct ExportMenuEntry`, `struct FindOccurrence`, `enum class DecodeType`, `struct FindExporterEntry`

Function-like declarations:
- `const std::vector<ExportMenuEntry>& getExportMenuEntries();`
- `const std::vector<FindExporterEntry>& getFindExporterEntries();`
- `void addExportMenuEntry(const UnlocalizedString &unlocalizedName, const impl::Callback &callback);`
- `void addFindExportFormatter(const UnlocalizedString &unlocalizedName, const std::string &fileExtension, const impl::FindExporterCallback &callback);`

## `libimhex/include/hex/api/content_registry/data_information.hpp`
Namespaces: `prv`, `ContentRegistry::DataInformation`, `impl`

Types: `class InformationSection`

Function-like declarations:
- `virtual ~InformationSection() = default;`
- `[[nodiscard]] const UnlocalizedString& getUnlocalizedName() const { return m_unlocalizedName;`
- `[[nodiscard]] const UnlocalizedString& getUnlocalizedDescription() const { return m_unlocalizedDescription;`
- `virtual void process(Task &task, prv::Provider *provider, Region region) = 0;`
- `virtual void reset() = 0;`
- `virtual void drawContent() = 0;`
- `[[nodiscard]] bool isValid() const { return m_valid;`
- `void markValid(bool valid = true) { m_valid = valid;`
- `[[nodiscard]] bool isEnabled() const { return m_enabled;`
- `void setEnabled(bool enabled) { m_enabled = enabled;`
- `[[nodiscard]] bool isAnalyzing() const { return m_analyzing;`
- `void setAnalyzing(bool analyzing) { m_analyzing = analyzing;`
- `virtual void load(const nlohmann::json &data);`
- `[[nodiscard]] virtual nlohmann::json store();`
- `[[nodiscard]] bool hasSettings() const { return m_hasSettings;`
- `const std::vector<CreateCallback>& getInformationSectionConstructors();`
- `void addInformationSectionCreator(const CreateCallback &callback);`
- `impl::addInformationSectionCreator([args...] { });`

## `libimhex/include/hex/api/content_registry/data_inspector.hpp`
Namespaces: `ContentRegistry::DataInspector`, `impl`

Types: `enum class NumberDisplayStyle`, `struct Entry`

Function-like declarations:
- `const std::vector<Entry>& getEntries();`
- `void add( const UnlocalizedString &unlocalizedName, size_t requiredSize, impl::GeneratorFunction displayGeneratorFunction, std::optional<impl::EditingFunction> editingFunction = std::nullopt );`
- `void add( const UnlocalizedString &unlocalizedName, size_t requiredSize, size_t maxSize, impl::GeneratorFunction displayGeneratorFunction, std::optional<impl::EditingFunction> editingFunction = std::nullopt );`
- `void drawMenuItems(const std::function<void()> &function);`

## `libimhex/include/hex/api/content_registry/data_processor.hpp`
Namespaces: `dp`, `ContentRegistry::DataProcessor`, `impl`

Types: `struct Entry`

Function-like declarations:
- `void add(const Entry &entry);`
- `const std::vector<Entry>& getEntries();`
- `add(impl::Entry { unlocalizedCategory, unlocalizedName, [unlocalizedName, ...args = std::forward<Args>(args)]() mutable { auto node = std::make_unique<T>(std::forward<Args>(args)...);`
- `void addSeparator();`

## `libimhex/include/hex/api/content_registry/diffing.hpp`
Namespaces: `prv`, `ContentRegistry::Diffing`, `impl`

Types: `enum class DifferenceType`, `class Algorithm`

Function-like declarations:
- `virtual ~Algorithm() = default;`
- `virtual std::vector<DiffTree> analyze(prv::Provider *providerA, prv::Provider *providerB) const = 0;`
- `const UnlocalizedString& getUnlocalizedName() const { return m_unlocalizedName;`
- `const UnlocalizedString& getUnlocalizedDescription() const { return m_unlocalizedDescription;`
- `const std::vector<std::unique_ptr<Algorithm>>& getAlgorithms();`
- `void addAlgorithm(std::unique_ptr<Algorithm> &&hash);`
- `impl::addAlgorithm(std::make_unique<T>(std::forward<Args>(args)...));`

## `libimhex/include/hex/api/content_registry/disassemblers.hpp`
Namespaces: `ContentRegistry::Disassemblers`, `impl`

Types: `struct Instruction`, `class Architecture`

Function-like declarations:
- `virtual ~Architecture() = default;`
- `virtual bool start() = 0;`
- `virtual void end() = 0;`
- `virtual std::optional<Instruction> disassemble(u64 imageBaseAddress, u64 instructionLoadAddress, u64 instructionDataAddress, std::span<const u8> code) = 0;`
- `virtual void drawSettings() = 0;`
- `[[nodiscard]] const std::string& getName() const { return m_name;`
- `void addArchitectureCreator(CreatorFunction function);`
- `const std::map<std::string, CreatorFunction>& getArchitectures();`
- `impl::addArchitectureCreator([...args = std::move(args)] { });`

## `libimhex/include/hex/api/content_registry/experiments.hpp`
Namespaces: `ContentRegistry::Experiments`, `impl`

Types: `struct Experiment`

Function-like declarations:
- `const std::map<std::string, Experiment>& getExperiments();`
- `void addExperiment( const std::string &experimentName, const UnlocalizedString &unlocalizedName, const UnlocalizedString &unlocalizedDescription = "" );`
- `void enableExperiement(const std::string &experimentName, bool enabled);`
- `[[nodiscard]] bool isExperimentEnabled(const std::string &experimentName);`

## `libimhex/include/hex/api/content_registry/file_type_handler.hpp`
Namespaces: `ContentRegistry::FileTypeHandler`, `impl`

Types: `struct Entry`

Function-like declarations:
- `const std::vector<Entry>& getEntries();`
- `void add(const std::vector<std::string> &extensions, const impl::Callback &callback);`

## `libimhex/include/hex/api/content_registry/hashes.hpp`
Namespaces: `prv`, `ContentRegistry::Hashes`, `impl`

Types: `class Hash`, `class Function`

Function-like declarations:
- `virtual ~Hash() = default;`
- `[[nodiscard]] Hash *getType() { return m_type;`
- `[[nodiscard]] const Hash *getType() const { return m_type;`
- `[[nodiscard]] const std::string& getName() const { return m_name;`
- `[[nodiscard]] virtual Function create(std::string name) = 0;`
- `[[nodiscard]] virtual nlohmann::json store() const = 0;`
- `virtual void load(const nlohmann::json &json) = 0;`
- `const std::vector<std::unique_ptr<Hash>>& getHashes();`
- `void add(std::unique_ptr<Hash> &&hash);`
- `impl::add(std::make_unique<T>(std::forward<Args>(args)...));`

## `libimhex/include/hex/api/content_registry/hex_editor.hpp`
Namespaces: `ContentRegistry::HexEditor`, `impl`

Types: `class DataVisualizer`, `struct MiniMapVisualizer`

Function-like declarations:
- `virtual ~DataVisualizer() = default;`
- `virtual void draw(u64 address, const u8 *data, size_t size, bool upperCase) = 0;`
- `virtual bool drawEditing(u64 address, u8 *data, size_t size, bool upperCase, bool startedEditing) = 0;`
- `[[nodiscard]] u16 getBytesPerCell() const { return m_bytesPerCell;`
- `[[nodiscard]] u16 getMaxCharsPerCell() const { return m_maxCharsPerCell;`
- `[[nodiscard]] const UnlocalizedString& getUnlocalizedName() const { return m_unlocalizedName;`
- `[[nodiscard]] static int DefaultTextInputFlags();`
- `bool drawDefaultScalarEditingTextBox(u64 address, const char *format, ImGuiDataType dataType, u8 *data, ImGuiInputTextFlags flags) const;`
- `bool drawDefaultTextEditingTextBox(u64 address, std::string &data, ImGuiInputTextFlags flags) const;`
- `void addDataVisualizer(std::shared_ptr<DataVisualizer> &&visualizer);`
- `const std::vector<std::shared_ptr<DataVisualizer>>& getVisualizers();`
- `const std::vector<std::shared_ptr<MiniMapVisualizer>>& getMiniMapVisualizers();`
- `std::shared_ptr<DataVisualizer> getVisualizerByName(const UnlocalizedString &unlocalizedName);`
- `void addMiniMapVisualizer(UnlocalizedString unlocalizedName, MiniMapVisualizer::Callback callback);`

## `libimhex/include/hex/api/content_registry/pattern_language.hpp`
Namespaces: `prv`, `ContentRegistry::PatternLanguage`, `impl`

Types: `struct FunctionDefinition`, `struct TypeDefinition`, `struct Visualizer`

Function-like declarations:
- `const std::map<std::string, Visualizer>& getVisualizers();`
- `const std::map<std::string, Visualizer>& getInlineVisualizers();`
- `const std::map<std::string, pl::api::PragmaHandler>& getPragmas();`
- `const std::vector<FunctionDefinition>& getFunctions();`
- `const std::vector<TypeDefinition>& getTypes();`
- `pl::PatternLanguage& getRuntime();`
- `std::mutex& getRuntimeLock();`
- `void configureRuntime(pl::PatternLanguage &runtime, prv::Provider *provider);`
- `void addPragma(const std::string &name, const pl::api::PragmaHandler &handler);`
- `void addFunction( const pl::api::Namespace &ns, const std::string &name, pl::api::FunctionParameterCount parameterCount, const pl::api::FunctionCallback &func );`
- `void addDangerousFunction( const pl::api::Namespace &ns, const std::string &name, pl::api::FunctionParameterCount parameterCount, const pl::api::FunctionCallback &func );`
- `void addType( const pl::api::Namespace &ns, const std::string &name, pl::api::FunctionParameterCount parameterCount, const pl::api::TypeCallback &func );`
- `void addVisualizer( const std::string &name, const impl::VisualizerFunctionCallback &function, pl::api::FunctionParameterCount parameterCount );`
- `void addInlineVisualizer( const std::string &name, const impl::VisualizerFunctionCallback &function, pl::api::FunctionParameterCount parameterCount );`

## `libimhex/include/hex/api/content_registry/provider.hpp`
Namespaces: `ContentRegistry::Provider`, `impl`

Types: `struct Entry`

Function-like declarations:
- `void addProviderName(const UnlocalizedString &unlocalizedName, const char *icon);`
- `void add(const std::string &typeName, ProviderCreationFunction creationFunction);`
- `const std::vector<Entry>& getEntries();`
- `auto typeName = provider.getTypeName();`
- `impl::add(typeName, []() -> std::unique_ptr<prv::Provider> { });`
- `impl::addProviderName(typeName, provider.getIcon());`

## `libimhex/include/hex/api/content_registry/reports.hpp`
Namespaces: `prv`, `ContentRegistry::Reports`, `impl`

Types: `struct ReportGenerator`

Function-like declarations:
- `const std::vector<ReportGenerator>& getGenerators();`
- `void addReportProvider(impl::Callback callback);`

## `libimhex/include/hex/api/content_registry/settings.hpp`
Namespaces: `ContentRegistry::Settings`, `Widgets`, `impl`

Types: `class Widget`, `class Interface`, `class Checkbox`, `class SliderInteger`, `class SliderFloat`, `class SliderDataSize`, `class ColorPicker`, `class DropDown`, `class TextBox`, `class FilePicker`, `class Label`, `struct Entry`, `struct SubCategory`, `struct Category`, `class SettingsValue`

Function-like declarations:
- `virtual ~Widget() = default;`
- `virtual bool draw(const std::string &name) = 0;`
- `virtual void load(const nlohmann::json &data) = 0;`
- `virtual nlohmann::json store() = 0;`
- `std::function<bool()> m_enabledCallback;`
- `std::function<void(Widget&)> m_changedCallback;`
- `Interface m_interface = Interface(this);`
- `bool draw(const std::string &name) override;`
- `void load(const nlohmann::json &data) override;`
- `nlohmann::json store() override;`
- `[[nodiscard]] bool isChecked() const { return m_value;`
- `[[nodiscard]] i32 getValue() const { return m_value;`
- `[[nodiscard]] float getValue() const { return m_value;`
- `explicit ColorPicker(ImColor defaultColor, ImGuiColorEditFlags flags = 0);`
- `[[nodiscard]] ImColor getColor() const;`
- `const nlohmann::json& getValue() const;`
- `const std::string& getValue() const { return m_value;`
- `nlohmann::json store() override { return {};`
- `void load();`
- `void store();`
- `void clear();`
- `const std::vector<Category>& getSettings();`
- `nlohmann::json& getSetting(const UnlocalizedString &unlocalizedCategory, const UnlocalizedString &unlocalizedName, const nlohmann::json &defaultValue);`
- `const nlohmann::json& getSettingsData();`
- `Widgets::Widget* add(const UnlocalizedString &unlocalizedCategory, const UnlocalizedString &unlocalizedSubCategory, const UnlocalizedString &unlocalizedName, std::unique_ptr<Widgets::Widget> &&widget);`
- `void printSettingReadError(const UnlocalizedString &unlocalizedCategory, const UnlocalizedString &unlocalizedName, const nlohmann::json::exception &e);`
- `void runOnChangeHandlers(const UnlocalizedString &unlocalizedCategory, const UnlocalizedString &unlocalizedName, const nlohmann::json &value);`
- `std::make_unique<T>(std::forward<decltype(args)>(args)...) )->getInterface();`
- `void setCategoryDescription(const UnlocalizedString &unlocalizedCategory, const UnlocalizedString &unlocalizedDescription);`
- `result = m_value.get<int>() != 0;`
- `auto setting = impl::getSetting(unlocalizedCategory, unlocalizedName, defaultValue);`
- `setting = setting.template get<int>() != 0;`
- `impl::printSettingReadError(unlocalizedCategory, unlocalizedName, e);`
- `impl::getSetting(unlocalizedCategory, unlocalizedName, value) = value;`
- `impl::runOnChangeHandlers(unlocalizedCategory, unlocalizedName, value);`
- `impl::store();`
- `u64 onChange(const UnlocalizedString &unlocalizedCategory, const UnlocalizedString &unlocalizedName, const OnChangeCallback &callback);`
- `u64 onSave(const OnSaveCallback &callback);`

## `libimhex/include/hex/api/content_registry/tools.hpp`
Namespaces: `ContentRegistry::Tools`, `impl`

Types: `struct Entry`

Function-like declarations:
- `const std::vector<Entry>& getEntries();`
- `void add(const UnlocalizedString &unlocalizedName, const char *icon, const impl::Callback &function);`

## `libimhex/include/hex/api/content_registry/user_interface.hpp`
Namespaces: `ContentRegistry::UserInterface`, `impl`

Types: `struct Icon`, `struct MainMenuItem`, `struct MenuItem`, `struct SidebarItem`, `struct TitleBarButton`, `struct WelcomeScreenQuickSettingsToggle`

Function-like declarations:
- `const std::multimap<u32, MainMenuItem>& getMainMenuItems();`
- `const std::multimap<u32, MenuItem>& getMenuItems();`
- `const std::vector<MenuItem*>& getToolbarMenuItems();`
- `std::multimap<u32, MenuItem>& getMenuItemsMutable();`
- `const std::vector<DrawCallback>& getWelcomeScreenEntries();`
- `const std::vector<DrawCallback>& getFooterItems();`
- `const std::vector<DrawCallback>& getToolbarItems();`
- `const std::vector<SidebarItem>& getSidebarItems();`
- `const std::vector<TitleBarButton>& getTitlebarButtons();`
- `const std::vector<WelcomeScreenQuickSettingsToggle>& getWelcomeScreenQuickSettingsToggles();`
- `void registerMainMenuItem(const UnlocalizedString &unlocalizedName, u32 priority);`
- `void addMenuItem( const std::vector<UnlocalizedString> &unlocalizedMainMenuNames, const Icon &icon, u32 priority, const Shortcut &shortcut, const impl::MenuCallback &function, const impl::EnabledCallback& enabledCallback, View *view );`
- `void addMenuItem( const std::vector<UnlocalizedString> &unlocalizedMainMenuNames, const Icon &icon, u32 priority, Shortcut shortcut, const impl::MenuCallback &function, const impl::EnabledCallback& enabledCallback = []{ return true;`
- `void addMenuItem( const std::vector<UnlocalizedString> &unlocalizedMainMenuNames, u32 priority, const Shortcut &shortcut, const impl::MenuCallback &function, const impl::EnabledCallback& enabledCallback = []{ return true;`
- `void addMenuItemSubMenu( std::vector<UnlocalizedString> unlocalizedMainMenuNames, u32 priority, const impl::MenuCallback &function, const impl::EnabledCallback& enabledCallback = []{ return true;`
- `void addMenuItemSubMenu( std::vector<UnlocalizedString> unlocalizedMainMenuNames, const char *icon, u32 priority, const impl::MenuCallback &function, const impl::EnabledCallback& enabledCallback = []{ return true;`
- `void addMenuItemSeparator(std::vector<UnlocalizedString> unlocalizedMainMenuNames, u32 priority, View *view = nullptr);`
- `void addWelcomeScreenEntry(const impl::DrawCallback &function);`
- `void addFooterItem(const impl::DrawCallback &function);`
- `void addToolbarItem(const impl::DrawCallback &function);`
- `void addMenuItemToToolbar(const UnlocalizedString &unlocalizedName, ImGuiCustomCol color);`
- `void updateToolbarItems();`
- `void addSidebarItem( const std::string &icon, const impl::DrawCallback &function, const impl::EnabledCallback &enabledCallback = []{ return true;`
- `void addTitleBarButton( const std::string &icon, ImGuiCustomCol color, const UnlocalizedString &unlocalizedTooltip, const impl::ClickCallback &function );`
- `void addWelcomeScreenQuickSettingsToggle( const std::string &icon, const UnlocalizedString &unlocalizedTooltip, bool defaultState, const impl::ToggleCallback &function );`
- `void addWelcomeScreenQuickSettingsToggle( const std::string &onIcon, const std::string &offIcon, const UnlocalizedString &unlocalizedTooltip, bool defaultState, const impl::ToggleCallback &function );`

## `libimhex/include/hex/api/content_registry/views.hpp`
Namespaces: `ContentRegistry::Views`, `impl`

Function-like declarations:
- `void add(std::unique_ptr<View> &&view);`
- `void setFullScreenView(std::unique_ptr<View> &&view);`
- `const std::map<UnlocalizedString, std::unique_ptr<View>>& getEntries();`
- `const std::unique_ptr<View>& getFullScreenView();`
- `View* getViewByName(const UnlocalizedString &unlocalizedName);`
- `View* getFocusedView();`

## `libimhex/include/hex/api/event_manager.hpp`
Namespaces: `impl`

Types: `struct event_name`, `class EventId`, `struct EventBase`, `struct Event`, `class EventManager`

Function-like declarations:
- `constexpr static auto Id = [] { return hex::impl::EventId(event_name_string);`
- `constexpr static auto ShouldLog = (should_log);`
- `EventManager::subscribe<event_name>(token, std::move(function));`
- `EventManager::unsubscribe(token);`
- `EventManager::unsubscribe<event_name>(token);`
- `EventManager::post<event_name>(std::forward<decltype(args)>(args)...);`
- `EventBase() noexcept = default;`
- `virtual ~EventBase() = default;`
- `log::error("An exception occurred while handling event {}: {}", wolv::type::getTypeName<E>(), e.what());`
- `std::lock_guard lock(getEventMutex());`
- `auto &events = getEvents();`
- `log::fatal("The token '{}' has already registered the same event ('{}')", token, wolv::type::getTypeName<E>());`
- `getTokenStore().insert({ token, subscribe<E>(std::move(function)) });`
- `getEvents().erase(token);`
- `unsubscribe(token, E::Id);`
- `const auto &[begin, end] = getEvents().equal_range(E::Id);`
- `(*static_cast<E *const>(event.get())).template call<E>(std::forward<decltype(args)>(args)...);`
- `log::debug("Event posted: '{}'", wolv::type::getTypeName<E>());`
- `getEvents().clear();`
- `getTokenStore().clear();`
- `static std::multimap<void *, EventList::iterator>& getTokenStore();`
- `static EventList& getEvents();`
- `static std::recursive_mutex& getEventMutex();`
- `static bool isAlreadyRegistered(void *token, impl::EventId id);`
- `static void unsubscribe(void *token, impl::EventId id);`

## `libimhex/include/hex/api/events/events_gui.hpp`
Namespaces: `hex`

Types: `struct GLFWwindow`

Events/requests:
- `EVENT_DEF(EventViewOpened, View*);`
- `EVENT_DEF(EventViewClosed, View*);`
- `EVENT_DEF(EventDPIChanged, float, float);`
- `EVENT_DEF(EventWindowFocused, bool);`
- `EVENT_DEF(EventWindowClosing, GLFWwindow*);`
- `EVENT_DEF(EventWindowDeinitializing, GLFWwindow*);`
- `EVENT_DEF(EventOSThemeChanged);`
- `EVENT_DEF_NO_LOG(EventFrameBegin);`
- `EVENT_DEF_NO_LOG(EventFrameEnd);`
- `EVENT_DEF_NO_LOG(EventSetTaskBarIconState, u32, u32, u32);`
- `EVENT_DEF_NO_LOG(EventImGuiElementRendered, ImGuiID, const std::array<float, 4>&);`

Function-like declarations:
- `* This event is called once at startup to signal native scale definition (by passing the same value twice). * On Windows OS, this event can also be posted if the window DPI has been changed. * * @param oldScale the old scale * @param newScale the current sc…`
- `* This event is used on Windows OS to display progress through the taskbar icon (the famous "green loading bar" * in the taskbar). * * @param progressState the progress state (converted from the TaskProgressState enum) * @param progressType the type of prog…`
- `* @param boundingBox the bounding box (composed of 4 floats) */ EVENT_DEF_NO_LOG(EventImGuiElementRendered, ImGuiID, const std::array<float, 4>&);`

## `libimhex/include/hex/api/events/events_interaction.hpp`
Namespaces: `hex`

Events/requests:
- `EVENT_DEF(EventFileLoaded, std::fs::path);`
- `EVENT_DEF(EventDataChanged, prv::Provider *);`
- `EVENT_DEF(EventHighlightingChanged);`
- `EVENT_DEF(EventRegionSelected, ImHexApi::HexEditor::ProviderRegion);`
- `EVENT_DEF(EventThemeChanged);`
- `EVENT_DEF(EventBookmarkCreated, ImHexApi::Bookmarks::Entry&);`
- `EVENT_DEF(EventPatchCreated, const u8*, u64, const PatchKind);`
- `EVENT_DEF(EventPatternEvaluating);`
- `EVENT_DEF(EventPatternExecuted, const std::string&);`
- `EVENT_DEF(EventPatternEditorChanged, const std::string&);`
- `EVENT_DEF(EventStoreContentDownloaded, const std::fs::path&);`
- `EVENT_DEF(EventStoreContentRemoved, const std::fs::path&);`
- `EVENT_DEF(EventAchievementUnlocked, const Achievement&);`
- `EVENT_DEF(EventSearchBoxClicked, u32);`
- `EVENT_DEF(EventFileDragged, bool);`
- `EVENT_DEF(EventFileDropped, std::fs::path);`

Function-like declarations:
- `* - an explicit provider reload, requested by the user (Ctrl+R) * - any user action that results in the creation of an "undo" stack action (generally a data modification) * * @param provider the Provider subject to the data change */ EVENT_DEF(EventDataChan…`
- `* As there are different behaviours depending on the click (left or right) done by the user, * this allows the consequences of said click to be registered in their own components. * * @param button the ImGuiMouseButton's value */ EVENT_DEF(EventSearchBoxCli…`

## `libimhex/include/hex/api/events/events_lifecycle.hpp`
Namespaces: `hex`

Types: `struct ImGuiTestEngine`

Events/requests:
- `EVENT_DEF(EventImHexStartupFinished);`
- `EVENT_DEF(EventCloseButtonPressed);`
- `EVENT_DEF(EventImHexClosing);`
- `EVENT_DEF(EventFirstLaunch);`
- `EVENT_DEF(EventAnySettingChanged);`
- `EVENT_DEF(EventAbnormalTermination, int);`
- `EVENT_DEF(EventImHexUpdated, SemanticVersion, SemanticVersion);`
- `EVENT_DEF(EventCrashRecovered, const std::exception &);`
- `EVENT_DEF(EventProjectOpened);`
- `EVENT_DEF(EventProjectSaved);`
- `EVENT_DEF(EventNativeMessageReceived, std::vector<u8>);`
- `EVENT_DEF(EventRegisterImGuiTests, ImGuiTestEngine*);`

Function-like declarations:
- `* This event allows for the launch of the ImHex tutorial (also called Out of Box experience). */ EVENT_DEF(EventFirstLaunch);`
- `* @brief Informs of the ImHex versions (and difference, if any) * * Called on every startup to inform subscribers of the two versions picked up: * - the version of the previous launch, gathered from the settings file * - the current version, gathered direct…`

## `libimhex/include/hex/api/events/events_provider.hpp`
Namespaces: `hex`, `prv`

Types: `class Provider`

Events/requests:
- `EVENT_DEF(EventProviderCreated, std::shared_ptr<prv::Provider>);`
- `EVENT_DEF(EventProviderOpened,  prv::Provider *);`
- `EVENT_DEF(EventProviderChanged, prv::Provider *, prv::Provider *);`
- `EVENT_DEF(EventProviderSaved,   prv::Provider *);`
- `EVENT_DEF(EventProviderClosing, prv::Provider *, bool *);`
- `EVENT_DEF(EventProviderClosed,  prv::Provider *);`
- `EVENT_DEF(EventProviderDeleted, prv::Provider *);`
- `EVENT_DEF(EventProviderDirtied, prv::Provider *);`
- `EVENT_DEF(EventProviderDataInserted, prv::Provider *, u64, u64);`
- `EVENT_DEF(EventProviderDataModified, prv::Provider *, u64, u64, const u8*);`
- `EVENT_DEF(EventProviderDataRemoved, prv::Provider *, u64, u64);`

Function-like declarations:
- `* This event is responsible for (optionally) initializing the provider and calling EventProviderOpened * (although the event can also be called manually without problem) */ EVENT_DEF(EventProviderCreated, std::shared_ptr<prv::Provider>);`
- `* If no initialization (Provider::skipLoadInterface() has been set), this event should be called manually * If skipLoadInterface failed, this event is not called * * @note this is not related to Provider::open() */ EVENT_DEF(EventProviderOpened, prv::Provid…`
- `* @brief Signals a change in provider (in-place) * * Note: if the provider was deleted, the new ("current") provider will be \`nullptr\` * * @param oldProvider the old provider * @param currentProvider the current provider */ EVENT_DEF(EventProviderChanged, p…`
- `* @param offset the data modification's offset (start address) * @param size the buffer's size * @param buffer the modified data written at this address */ EVENT_DEF(EventProviderDataModified, prv::Provider *, u64, u64, const u8*);`
- `* @param offset the deletion offset (start address) * @param size the deleted data's size */ EVENT_DEF(EventProviderDataRemoved, prv::Provider *, u64, u64);`

## `libimhex/include/hex/api/events/requests_gui.hpp`
Namespaces: `hex`

Events/requests:
- `EVENT_DEF(RequestOpenWindow, std::string);`
- `EVENT_DEF(RequestUpdateWindowTitle);`
- `EVENT_DEF(RequestChangeTheme, std::string);`
- `EVENT_DEF(RequestOpenPopup, std::string);`
- `EVENT_DEF(RequestSetPostProcessingShader, std::string, std::string);`

Function-like declarations:
- `* @brief Requests a theme type (light or dark) change * * @param themeType either \`Light\` or \`Dark\` */ EVENT_DEF(RequestChangeTheme, std::string);`

## `libimhex/include/hex/api/events/requests_interaction.hpp`
Namespaces: `pl::ptrn`, `hex`

Events/requests:
- `EVENT_DEF(RequestHexEditorSelectionChange, ImHexApi::HexEditor::ProviderRegion);`
- `EVENT_DEF(RequestPatternEditorSelectionChange, u32, u32);`
- `EVENT_DEF(RequestJumpToPattern, const pl::ptrn::Pattern*);`
- `EVENT_DEF(RequestAddBookmark, Region, std::string, std::string, color_t, u64*);`
- `EVENT_DEF(RequestRemoveBookmark, u64);`
- `EVENT_DEF(RequestSetPatternLanguageCode, std::string);`
- `EVENT_DEF(RequestTriggerPatternEvaluation);`
- `EVENT_DEF(RequestOpenFile, std::fs::path);`
- `EVENT_DEF(RequestAddVirtualFile, std::fs::path, std::vector<u8>, Region);`
- `EVENT_DEF(RequestOpenCommandPalette);`

## `libimhex/include/hex/api/events/requests_lifecycle.hpp`
Namespaces: `hex`

Events/requests:
- `EVENT_DEF(RequestAddInitTask, std::string, bool, std::function<bool()>);`
- `EVENT_DEF(RequestAddExitTask, std::string, std::function<bool()>);`
- `EVENT_DEF(RequestCloseImHex, bool);`
- `EVENT_DEF(RequestRestartImHex);`
- `EVENT_DEF(RequestInitThemeHandlers);`
- `EVENT_DEF(SendMessageToMainInstance, const std::string, const std::vector<u8>&);`

Function-like declarations:
- `* @param isAsync Whether the task is asynchronous (true if yes) * @param callbackFunction The function to call to execute the task */ EVENT_DEF(RequestAddInitTask, std::string, bool, std::function<bool()>);`
- `* If there are no questions (bool set to true), ImHex closes immediately. * If set to false, there is a procedure run to prompt a confirmation to the user. * * @param noQuestions true if no questions */ EVENT_DEF(RequestCloseImHex, bool);`
- `* (\`EventImHexStartupFinished\`). * * FIXME: change the name so that it is prefixed with "Request" like every other request. * * @param name the subcommand's name * @param data the subcommand's data */ EVENT_DEF(SendMessageToMainInstance, const std::string, …`

## `libimhex/include/hex/api/events/requests_provider.hpp`
Namespaces: `hex`

Events/requests:
- `EVENT_DEF(RequestCreateProvider, std::string, bool, bool, std::shared_ptr<hex::prv::Provider> *);`
- `EVENT_DEF(RequestOpenProvider, std::shared_ptr<prv::Provider>);`
- `EVENT_DEF(MovePerProviderData, prv::Provider *, prv::Provider *);`

## `libimhex/include/hex/api/imhex_api/bookmarks.hpp`
Namespaces: `ImHexApi::Bookmarks`

Types: `struct Entry`

Function-like declarations:
- `u64 add(u64 address, size_t size, const std::string &name, const std::string &comment, color_t color = 0x00000000);`
- `u64 add(Region region, const std::string &name, const std::string &comment, color_t color = 0x00000000);`
- `void remove(u64 id);`

## `libimhex/include/hex/api/imhex_api/fonts.hpp`
Namespaces: `ImHexApi::Fonts`, `impl`

Types: `struct ImFont`, `struct Offset`, `struct MergeFont`, `class Font`, `struct FontDefinition`

Function-like declarations:
- `explicit Font(UnlocalizedString fontName);`
- `void push(float size = 0.0F) const;`
- `void pushBold(float size = 0.0F) const;`
- `void pushItalic(float size = 0.0F) const;`
- `void pop() const;`
- `[[nodiscard]] operator ImFont*() const;`
- `[[nodiscard]] const UnlocalizedString& getUnlocalizedName() const { return m_fontName;`
- `void push(float size, ImFont *font) const;`
- `const std::vector<MergeFont>& getMergeFonts();`
- `std::map<UnlocalizedString, FontDefinition>& getFontDefinitions();`
- `void registerMergeFont(const std::string &name, const std::span<const u8> &data, Offset offset = {}, std::optional<float> fontSizeMultiplier = std::nullopt);`
- `void registerFont(const Font& font);`
- `FontDefinition getFont(const UnlocalizedString &fontName);`
- `void setDefaultFont(const Font& font);`
- `const Font& getDefaultFont();`
- `float getDpi();`
- `float pixelsToPoints(float pixels);`
- `float pointsToPixels(float points);`

## `libimhex/include/hex/api/imhex_api/hex_editor.hpp`
Namespaces: `prv`, `ImHexApi::HexEditor`, `impl`

Types: `class Highlighting`, `class Tooltip`, `struct ProviderRegion`

Function-like declarations:
- `Highlighting() = default;`
- `Highlighting(Region region, color_t color);`
- `[[nodiscard]] const Region& getRegion() const { return m_region;`
- `[[nodiscard]] const color_t& getColor() const { return m_color;`
- `Tooltip() = default;`
- `Tooltip(Region region, std::string value, color_t color);`
- `[[nodiscard]] const std::string& getValue() const { return m_value;`
- `[[nodiscard]] prv::Provider *getProvider() const { return this->provider;`
- `[[nodiscard]] Region getRegion() const { return { this->address, this->size };`
- `const std::map<u32, Highlighting>& getBackgroundHighlights();`
- `const std::map<u32, HighlightingFunction>& getBackgroundHighlightingFunctions();`
- `const std::map<u32, Highlighting>& getForegroundHighlights();`
- `const std::map<u32, HighlightingFunction>& getForegroundHighlightingFunctions();`
- `const std::map<u32, HoveringFunction>& getHoveringFunctions();`
- `const std::map<u32, Tooltip>& getTooltips();`
- `const std::map<u32, TooltipFunction>& getTooltipFunctions();`
- `void setCurrentSelection(const std::optional<ProviderRegion> &region);`
- `void setHoveredRegion(const prv::Provider *provider, const Region &region);`
- `u32 addBackgroundHighlight(const Region &region, color_t color);`
- `void removeBackgroundHighlight(u32 id);`
- `u32 addForegroundHighlight(const Region &region, color_t color);`
- `void removeForegroundHighlight(u32 id);`
- `u32 addTooltip(Region region, std::string value, color_t color);`
- `void removeTooltip(u32 id);`
- `u32 addTooltipProvider(TooltipFunction function);`
- `void removeTooltipProvider(u32 id);`
- `u32 addBackgroundHighlightingProvider(const impl::HighlightingFunction &function);`
- `void removeBackgroundHighlightingProvider(u32 id);`
- `u32 addForegroundHighlightingProvider(const impl::HighlightingFunction &function);`
- `void removeForegroundHighlightingProvider(u32 id);`
- `u32 addHoverHighlightProvider(const impl::HoveringFunction &function);`
- `void removeHoverHighlightProvider(u32 id);`
- `bool isSelectionValid();`
- `void clearSelection();`
- `std::optional<ProviderRegion> getSelection();`
- `void setSelection(const Region &region, prv::Provider *provider = nullptr);`
- `void setSelection(const ProviderRegion &region);`
- `void setSelection(u64 address, size_t size, prv::Provider *provider = nullptr);`
- `void addVirtualFile(const std::string &path, std::vector<u8> data, Region region = Region::Invalid());`
- `const std::optional<Region>& getHoveredRegion(const prv::Provider *provider);`

## `libimhex/include/hex/api/imhex_api/messaging.hpp`
Namespaces: `ImHexApi::Messaging`, `impl`

Function-like declarations:
- `const std::map<std::string, MessagingHandler>& getHandlers();`
- `void runHandler(const std::string &eventName, const std::vector<u8> &args);`
- `void registerHandler(const std::string &eventName, const impl::MessagingHandler &handler);`

## `libimhex/include/hex/api/imhex_api/provider.hpp`
Namespaces: `ImHexApi::Provider`, `impl`

Function-like declarations:
- `void resetClosingProvider();`
- `std::set<prv::Provider*> getClosingProviders();`
- `prv::Provider *get();`
- `std::vector<prv::Provider*> getProviders();`
- `void setCurrentProvider(i64 index);`
- `void setCurrentProvider(NonNull<prv::Provider*> provider);`
- `i64 getCurrentProviderIndex();`
- `bool isValid();`
- `void markDirty();`
- `void resetDirty();`
- `bool isDirty();`
- `* @param skipLoadInterface Whether to skip the provider's loading interface (see property documentation) * @param select Whether to select the provider after adding it */ void add(std::shared_ptr<prv::Provider> &&provider, bool skipLoadInterface = false, bo…`
- `add(std::make_unique<T>(std::forward<decltype(args)>(args)...));`
- `void remove(prv::Provider *provider, bool noQuestions = false);`
- `* @param skipLoadInterface Whether to skip the provider's loading interface (see property documentation) * @param select Whether to select the provider after adding it */ std::shared_ptr<prv::Provider> createProvider( const UnlocalizedString &unlocalizedNam…`
- `void openProvider(std::shared_ptr<prv::Provider> provider);`

## `libimhex/include/hex/api/imhex_api/system.hpp`
Namespaces: `impl`, `ImHexApi::System`

Types: `struct ImVec2`, `struct ImFontAtlas`, `struct GLFWwindow`, `class AutoResetBase`, `struct ProgramArguments`, `struct InitialWindowProperties`, `enum class TaskProgressState`, `enum class TaskProgressType`, `struct LinuxDistro`, `enum class UpdateType`

Function-like declarations:
- `void setMainInstanceStatus(bool status);`
- `void setMainWindowPosition(i32 x, i32 y);`
- `void setMainWindowSize(u32 width, u32 height);`
- `void setMainDockSpaceId(ImGuiID id);`
- `void setMainWindowHandle(GLFWwindow *window);`
- `void setGlobalScale(float scale);`
- `void setNativeScale(float scale);`
- `void setBorderlessWindowMode(bool enabled);`
- `void setMultiWindowMode(bool enabled);`
- `void setInitialWindowProperties(InitialWindowProperties properties);`
- `void setGPUVendor(const std::string &vendor);`
- `void setGLRenderer(const std::string &renderer);`
- `void setGLVersion(SemanticVersion version);`
- `void addInitArgument(const std::string &key, const std::string &value = { });`
- `void setLastFrameTime(double time);`
- `bool isWindowResizable();`
- `void addAutoResetObject(hex::impl::AutoResetBase *object);`
- `void removeAutoResetObject(hex::impl::AutoResetBase *object);`
- `void cleanup();`
- `bool frameRateUnlockRequested();`
- `void resetFrameRateUnlockRequested();`
- `void closeImHex(bool noQuestions = false);`
- `void restartImHex();`
- `void setTaskBarProgress(TaskProgressState state, TaskProgressType type, u32 progress);`
- `float getTargetFPS();`
- `void setTargetFPS(float fps);`
- `float getGlobalScale();`
- `float getNativeScale();`
- `float getBackingScaleFactor();`
- `ImVec2 getMainWindowPosition();`
- `ImVec2 getMainWindowSize();`
- `ImGuiID getMainDockSpaceId();`
- `GLFWwindow* getMainWindowHandle();`
- `bool isBorderlessWindowModeEnabled();`
- `bool isMultiWindowModeEnabled();`
- `const std::map<std::string, std::string>& getInitArguments();`
- `std::string getInitArgument(const std::string &key);`
- `void enableSystemThemeDetection(bool enabled);`
- `bool usesSystemThemeDetection();`
- `const std::vector<std::fs::path>& getAdditionalFolderPaths();`
- `void setAdditionalFolderPaths(const std::vector<std::fs::path> &paths);`
- `const std::string& getGPUVendor();`
- `const std::string& getGLRenderer();`
- `const SemanticVersion& getGLVersion();`
- `bool isCorporateEnvironment();`
- `bool isPortableVersion();`
- `std::string getOSName();`
- `std::string getOSVersion();`
- `std::string getArchitecture();`
- `std::optional<LinuxDistro> getLinuxDistro();`
- `const SemanticVersion& getImHexVersion();`
- `std::string getCommitHash(bool longHash = false);`
- `std::string getCommitBranch();`
- `std::optional<std::chrono::system_clock::time_point> getBuildTime();`
- `bool isDebugBuild();`
- `bool isNightlyBuild();`
- `std::optional<std::string> checkForUpdate();`
- `bool updateImHex(UpdateType updateType);`
- `void addStartupTask(const std::string &name, bool async, const std::function<bool()> &function);`
- `double getLastFrameTime();`
- `void setWindowResizable(bool resizable);`
- `bool isMainInstance();`
- `std::optional<InitialWindowProperties> getInitialWindowProperties();`
- `void* getLibImHexModuleHandle();`
- `void addMigrationRoutine(SemanticVersion migrationVersion, std::function<void()> function);`
- `void unlockFrameRate();`
- `void setPostProcessingShader(const std::string &vertexShader, const std::string &fragmentShader);`

## `libimhex/include/hex/api/layout_manager.hpp`

Types: `struct ImGuiTextBuffer`, `class LayoutManager`, `struct Layout`

Function-like declarations:
- `static void save(const std::string &name);`
- `static void load(const std::fs::path &path);`
- `static std::string saveToString();`
- `static void loadFromString(const std::string &content);`
- `static const std::vector<Layout> &getLayouts();`
- `static void removeLayout(const std::string &name);`
- `static void process();`
- `static void reload();`
- `static void reset();`
- `static bool isLayoutLocked();`
- `static void lockLayout(bool locked);`
- `static void closeAllViews();`
- `static void registerLoadCallback(const LoadCallback &callback);`
- `static void registerStoreCallback(const StoreCallback &callback);`
- `static void onStore(ImGuiTextBuffer *buffer);`
- `static void onLoad(std::string_view line);`
- `LayoutManager() = default;`

## `libimhex/include/hex/api/localization_manager.hpp`
Namespaces: `LocalizationManager`, `fmt`

Types: `struct UnlocalizedString`, `struct PathEntry`, `struct LanguageDefinition`, `class LangConst`, `class Lang`, `struct std`

Function-like declarations:
- `std::function<std::string_view(const std::string &path)> callback;`
- `void addLanguages(const std::string_view &languageList, std::function<std::string_view(const std::string &path)> callback);`
- `void setLanguage(const LanguageId &languageId);`
- `[[nodiscard]] const LanguageId& getSelectedLanguageId();`
- `[[nodiscard]] const std::string& get(const LanguageId& languageId, const UnlocalizedString &unlocalizedString);`
- `[[nodiscard]] const std::map<LanguageId, LanguageDefinition>& getLanguageDefinitions();`
- `[[nodiscard]] const LanguageDefinition& getLanguageDefinition(const LanguageId &languageId);`
- `explicit Lang(const char *unlocalizedString);`
- `explicit Lang(const std::string &unlocalizedString);`
- `explicit(false) Lang(const LangConst &localizedString);`
- `explicit Lang(const UnlocalizedString &unlocalizedString);`
- `explicit Lang(std::string_view unlocalizedString);`
- `[[nodiscard]] operator std::string() const;`
- `[[nodiscard]] operator std::string_view() const;`
- `[[nodiscard]] operator const char *() const;`
- `const char* get() const;`
- `constexpr u64 m = std::numeric_limits<std::uint32_t>::max() - 4;`
- `total = (total + currentMultiplier * c) % m;`
- `currentMultiplier = (currentMultiplier * p) % m;`
- `UnlocalizedString() = default;`
- `UnlocalizedString(const Lang& arg) = delete;`
- `UnlocalizedString(UnlocalizedString &&) = default;`
- `UnlocalizedString(const UnlocalizedString &) = default;`
- `UnlocalizedString &operator=(const UnlocalizedString &) = default;`
- `UnlocalizedString &operator=(UnlocalizedString &&) = default;`
- `UnlocalizedString &operator=(const std::string &string) { m_unlocalizedString = string;`
- `UnlocalizedString &operator=(std::string &&string) { m_unlocalizedString = std::move(string);`
- `auto operator<=>(const UnlocalizedString &) const = default;`

## `libimhex/include/hex/api/plugin_manager.hpp`

Types: `struct ImGuiContext`, `struct SubCommand`, `enum class Type`, `struct Feature`, `struct PluginFunctions`, `class Plugin`, `class PluginManager`

Function-like declarations:
- `std::function<void(const std::vector<std::string>&)> callback;`
- `explicit Plugin(const std::fs::path &path);`
- `explicit Plugin(const std::string &name, const PluginFunctions &functions);`
- `Plugin(const Plugin &) = delete;`
- `Plugin(Plugin &&other) noexcept;`
- `~Plugin();`
- `Plugin& operator=(const Plugin &) = delete;`
- `Plugin& operator=(Plugin &&other) noexcept;`
- `[[nodiscard]] bool initializePlugin() const;`
- `[[nodiscard]] std::string getPluginName() const;`
- `[[nodiscard]] std::string getPluginAuthor() const;`
- `[[nodiscard]] std::string getPluginDescription() const;`
- `[[nodiscard]] std::string getCompatibleVersion() const;`
- `void setImGuiContext(ImGuiContext *ctx) const;`
- `[[nodiscard]] const std::fs::path &getPath() const;`
- `[[nodiscard]] bool isLoaded() const;`
- `[[nodiscard]] bool isValid() const;`
- `[[nodiscard]] bool isInitialized() const;`
- `[[nodiscard]] bool isBuiltinPlugin() const;`
- `[[nodiscard]] std::span<SubCommand> getSubCommands() const;`
- `[[nodiscard]] std::span<Feature> getFeatures() const;`
- `[[nodiscard]] bool isLibraryPlugin() const;`
- `[[nodiscard]] bool wasAddedManually() const;`
- `void setEnabled(bool enabled);`
- `[[nodiscard]] void *getPluginFunction(const std::string &symbol) const;`
- `PluginManager() = delete;`
- `static bool load();`
- `static bool load(const std::fs::path &pluginFolder);`
- `static bool loadLibraries();`
- `static bool loadLibraries(const std::fs::path &libraryFolder);`
- `static void unload();`
- `static void reload();`
- `static void initializeNewPlugins();`
- `static void addLoadPath(const std::fs::path &path);`
- `static void addPlugin(const std::string &name, PluginFunctions functions);`
- `static Plugin* getPlugin(const std::string &name);`
- `static const std::list<Plugin>& getPlugins();`
- `static const std::vector<std::fs::path>& getPluginPaths();`
- `static const std::vector<std::fs::path>& getPluginLoadPaths();`
- `static bool isPluginLoaded(const std::fs::path &path);`
- `static void setPluginEnabled(const Plugin &plugin, bool enabled);`
- `static std::list<Plugin>& getPluginsMutable();`

## `libimhex/include/hex/api/project_file_manager.hpp`
Namespaces: `prv`

Types: `class Provider`, `class ProjectFile`, `struct Handler`, `struct ProviderHandler`

Function-like declarations:
- `static void setProjectFunctions( const std::function<bool(const std::fs::path&)> &loadFun, const std::function<bool(std::optional<std::fs::path>, bool)> &storeFun );`
- `static bool load(const std::fs::path &filePath);`
- `static bool store(std::optional<std::fs::path> filePath = std::nullopt, bool updateLocation = true);`
- `static bool hasPath();`
- `static void clearPath();`
- `static std::fs::path getPath();`
- `static void setPath(const std::fs::path &path);`
- `static void registerHandler(const Handler &handler);`
- `static void registerPerProviderHandler(const ProviderHandler &handler);`
- `static const std::vector<Handler>& getHandlers();`
- `static const std::vector<ProviderHandler>& getProviderHandlers();`
- `ProjectFile() = default;`

## `libimhex/include/hex/api/shortcut_manager.hpp`

Types: `struct ImGuiWindow`, `struct KeyEquivalent`, `class View`, `class Key`, `class Shortcut`, `class ShortcutManager`, `struct ShortcutEntry`

Function-like declarations:
- `constexpr Key() = default;`
- `bool operator==(const Key &) const = default;`
- `auto operator<=>(const Key &) const = default;`
- `[[nodiscard]] constexpr u32 getKeyCode() const { return m_key;`
- `constexpr static auto CTRL = Key(static_cast<Keys>(0x0100'0000));`
- `constexpr static auto ALT = Key(static_cast<Keys>(0x0200'0000));`
- `constexpr static auto SHIFT = Key(static_cast<Keys>(0x0400'0000));`
- `constexpr static auto SUPER = Key(static_cast<Keys>(0x0800'0000));`
- `constexpr static auto CurrentView = Key(static_cast<Keys>(0x1000'0000));`
- `constexpr static auto AllowWhileTyping = Key(static_cast<Keys>(0x2000'0000));`
- `constexpr static auto CTRLCMD = Key(static_cast<Keys>(0x4000'0000));`
- `constexpr static auto ShowOnWelcomeScreen = Key(static_cast<Keys>(0x8000'0000));`
- `Shortcut() = default;`
- `Shortcut(Keys key);`
- `explicit Shortcut(std::set<Key> keys);`
- `Shortcut(const Shortcut &other) = default;`
- `Shortcut(Shortcut &&) noexcept = default;`
- `constexpr static auto None = Keys(0);`
- `Shortcut& operator=(const Shortcut &other) = default;`
- `Shortcut& operator=(Shortcut &&) noexcept = default;`
- `Shortcut operator+(const Key &other) const;`
- `Shortcut &operator+=(const Key &other);`
- `bool operator<(const Shortcut &other) const;`
- `bool operator==(const Shortcut &other) const;`
- `bool isLocal() const;`
- `std::string toString() const;`
- `KeyEquivalent toKeyEquivalent() const;`
- `const std::set<Key>& getKeys() const;`
- `bool has(Key key) const;`
- `bool matches(const Shortcut &other) const;`
- `Shortcut operator+(const Key &lhs, const Key &rhs);`
- `static void addGlobalShortcut(const Shortcut &shortcut, const std::vector<UnlocalizedString> &unlocalizedName, const Callback &callback, const EnabledCallback &enabledCallback = []{ return true;`
- `static void addGlobalShortcut(const Shortcut &shortcut, const UnlocalizedString &unlocalizedName, const Callback &callback, const EnabledCallback &enabledCallback = []{ return true;`
- `static void addShortcut(View *view, const Shortcut &shortcut, const std::vector<UnlocalizedString> &unlocalizedName, const Callback &callback, const EnabledCallback &enabledCallback = []{ return true;`
- `static void addShortcut(View *view, const Shortcut &shortcut, const UnlocalizedString &unlocalizedName, const Callback &callback, const EnabledCallback &enabledCallback = []{ return true;`
- `static void process(const View *currentView, bool ctrl, bool alt, bool shift, bool super, bool focused, u32 keyCode);`
- `static void processGlobals(bool ctrl, bool alt, bool shift, bool super, u32 keyCode);`
- `static bool runShortcut(const Shortcut &shortcut, const View *view = nullptr);`
- `static void clearShortcuts();`
- `static Shortcut getShortcutByName(const std::vector<UnlocalizedString> &unlocalizedName, const View *view = nullptr);`
- `static void resumeShortcuts();`
- `static void pauseShortcuts();`
- `static void enableMacOSMode();`
- `[[nodiscard]] static std::optional<UnlocalizedString> getLastActivatedMenu();`
- `static void resetLastActivatedMenu();`
- `[[nodiscard]] static std::optional<Shortcut> getPreviousShortcut();`
- `[[nodiscard]] static std::vector<ShortcutEntry> getGlobalShortcuts();`
- `[[nodiscard]] static std::vector<ShortcutEntry> getViewShortcuts(const View *view);`
- `[[nodiscard]] static bool updateShortcut(const Shortcut &oldShortcut, Shortcut newShortcut, View *view = nullptr);`

## `libimhex/include/hex/api/task_manager.hpp`

Types: `class TaskHolder`, `class TaskManager`, `class Task`, `struct TaskInterruptor`

Function-like declarations:
- `Task() = default;`
- `Task(const UnlocalizedString &unlocalizedName, u64 maxValue, bool background, bool blocking, std::function<void(Task &)> function);`
- `Task(const Task&) = delete;`
- `Task(Task &&other) noexcept;`
- `~Task();`
- `void update(u64 value);`
- `void update() const;`
- `void increment();`
- `void setMaxValue(u64 value);`
- `void interrupt();`
- `void setInterruptCallback(std::function<void()> callback);`
- `[[nodiscard]] bool isBackgroundTask() const;`
- `[[nodiscard]] bool isBlocking() const;`
- `[[nodiscard]] bool isFinished() const;`
- `[[nodiscard]] bool hadException() const;`
- `[[nodiscard]] bool wasInterrupted() const;`
- `[[nodiscard]] bool shouldInterrupt() const;`
- `void clearException();`
- `[[nodiscard]] std::string getExceptionMessage() const;`
- `[[nodiscard]] const UnlocalizedString &getUnlocalizedName();`
- `[[nodiscard]] u64 getValue() const;`
- `[[nodiscard]] u64 getMaxValue() const;`
- `void wait() const;`
- `void finish();`
- `void interruption();`
- `void exception(const char *message);`
- `std::function<void()> m_interruptCallback;`
- `std::function<void(Task &)> m_function;`
- `trace::disableExceptionCaptureForCurrentThread();`
- `virtual ~TaskInterruptor() = default;`
- `TaskHolder() = default;`
- `[[nodiscard]] bool isRunning() const;`
- `[[nodiscard]] u32 getProgress() const;`
- `void interrupt() const;`
- `TaskManager() = delete;`
- `static void init();`
- `static void exit();`
- `static TaskHolder createTask(const UnlocalizedString &unlocalizedName, u64 maxValue, std::function<void(Task &)> function);`
- `static TaskHolder createTask(const UnlocalizedString &unlocalizedName, u64 maxValue, std::function<void()> function);`
- `static TaskHolder createBackgroundTask(const UnlocalizedString &unlocalizedName, std::function<void(Task &)> function);`
- `static TaskHolder createBackgroundTask(const UnlocalizedString &unlocalizedName, std::function<void()> function);`
- `static TaskHolder createBlockingTask(const UnlocalizedString &unlocalizedName, u64 maxValue, std::function<void(Task &)> function);`
- `static TaskHolder createBlockingTask(const UnlocalizedString &unlocalizedName, u64 maxValue, std::function<void()> function);`
- `static void doLater(const std::function<void()> &function);`
- `static void doLaterOnce(const std::function<void()> &function, std::source_location location = std::source_location::current());`
- `static void runWhenTasksFinished(const std::function<void()> &function);`
- `static void setCurrentThreadName(const std::string &name);`
- `static std::string_view getCurrentThreadName();`
- `static void setMainThreadId(std::thread::id threadId);`
- `static bool isMainThread();`
- `static void collectGarbage();`
- `static Task& getCurrentTask();`
- `static size_t getRunningTaskCount();`
- `static size_t getRunningBackgroundTaskCount();`
- `static size_t getRunningBlockingTaskCount();`
- `static const std::list<std::shared_ptr<Task>>& getRunningTasks();`
- `static void runDeferredCalls();`
- `static TaskHolder createTask(const UnlocalizedString &unlocalizedName, u64 maxValue, bool background, bool blocking, std::function<void(Task &)> function);`

## `libimhex/include/hex/api/theme_manager.hpp`

Types: `class ThemeManager`, `struct Style`, `struct ThemeHandler`, `struct StyleHandler`

Function-like declarations:
- `static void changeTheme(std::string name);`
- `static void addTheme(const std::string &content);`
- `static void addThemeHandler(const std::string &name, const ColorMap &colorMap, const std::function<ImColor(u32)> &getFunction, const std::function<void(u32, ImColor)> &setFunction);`
- `static void addStyleHandler(const std::string &name, const StyleMap &styleMap);`
- `static void reapplyCurrentTheme();`
- `static std::vector<std::string> getThemeNames();`
- `static const std::string &getImageTheme();`
- `static std::optional<ImColor> parseColorString(const std::string &colorString);`
- `static nlohmann::json exportCurrentTheme(const std::string &name);`
- `static void reset();`
- `static void setAccentColor(const ImColor &color);`
- `std::function<ImColor(u32)> getFunction;`
- `std::function<void(u32, ImColor)> setFunction;`
- `static const std::map<std::string, ThemeHandler>& getThemeHandlers();`
- `static const std::map<std::string, StyleHandler>& getStyleHandlers();`
- `ThemeManager() = default;`

## `libimhex/include/hex/api/tutorial_manager.hpp`

Types: `class TutorialManager`, `enum class Position`, `struct Tutorial`, `struct Step`, `struct Highlight`, `struct Message`

Function-like declarations:
- `Tutorial() = delete;`
- `Step& addHighlight(const UnlocalizedString &unlocalizedText, std::initializer_list<std::variant<Lang, std::string, int>> &&ids);`
- `Step& addHighlight(std::initializer_list<std::variant<Lang, std::string, int>> &&ids);`
- `Step& setMessage(const UnlocalizedString &unlocalizedTitle, const UnlocalizedString &unlocalizedMessage, Position position = Position::None);`
- `Step& allowSkip();`
- `Step& onAppear(std::function<void()> callback);`
- `Step& onComplete(std::function<void()> callback);`
- `bool isCurrent() const;`
- `void complete() const;`
- `void addHighlights() const;`
- `void removeHighlights() const;`
- `void advance(i32 steps = 1) const;`
- `std::function<void()> m_onAppear, m_onComplete;`
- `Step& addStep();`
- `const UnlocalizedString& getUnlocalizedName() const { return m_unlocalizedName;`
- `const UnlocalizedString& getUnlocalizedDescription() const { return m_unlocalizedDescription;`
- `void start();`
- `decltype(m_steps)::iterator m_currentStep, m_latestStep;`
- `static void init();`
- `static const std::map<std::string, Tutorial>& getTutorials();`
- `static std::map<std::string, Tutorial>::iterator getCurrentTutorial();`
- `static Tutorial& createTutorial(const UnlocalizedString &unlocalizedName, const UnlocalizedString &unlocalizedDescription);`
- `static void startTutorial(const UnlocalizedString &unlocalizedName);`
- `static void startHelpHover();`
- `static void addInteractiveHelpText(std::initializer_list<std::variant<Lang, std::string, int>> &&ids, UnlocalizedString unlocalizedString);`
- `static void addInteractiveHelpLink(std::initializer_list<std::variant<Lang, std::string, int>> &&ids, std::string link);`
- `static void setLastItemInteractiveHelpPopup(std::function<void()> callback);`
- `static void setLastItemInteractiveHelpLink(std::string link);`
- `static void drawTutorial();`
- `static void reset();`
- `TutorialManager() = delete;`
- `static void drawHighlights();`
- `static void drawMessageBox(std::optional<Tutorial::Step::Message> message);`

## `libimhex/include/hex/api/workspace_manager.hpp`

Types: `class WorkspaceManager`, `struct Workspace`

Function-like declarations:
- `static void createWorkspace(const std::string &name, const std::string &layout = "");`
- `static void switchWorkspace(const std::string &name);`
- `static void importFromFile(const std::fs::path &path);`
- `static bool exportToFile(std::fs::path path = {}, std::string workspaceName = {}, bool builtin = false);`
- `static void removeWorkspace(const std::string &name);`
- `static const std::map<std::string, Workspace>& getWorkspaces();`
- `static const std::map<std::string, Workspace>::iterator& getCurrentWorkspace();`
- `static void reset();`
- `static void reload();`
- `static void process();`
- `WorkspaceManager() = default;`

## `libimhex/include/hex/api_urls.hpp`

## `libimhex/include/hex/data_processor/attribute.hpp`
Namespaces: `hex::dp`

Types: `class Node`, `class Attribute`, `enum class Type`, `enum class IOType`

Function-like declarations:
- `Attribute(IOType ioType, Type type, UnlocalizedString unlocalizedName);`
- `~Attribute();`
- `[[nodiscard]] int getId() const { return m_id;`
- `void setId(int id) { m_id = id;`
- `[[nodiscard]] IOType getIOType() const { return m_ioType;`
- `[[nodiscard]] Type getType() const { return m_type;`
- `[[nodiscard]] const UnlocalizedString &getUnlocalizedName() const { return m_unlocalizedName;`
- `void addConnectedAttribute(int linkId, Attribute *to) { m_connectedAttributes.insert({ linkId, to });`
- `void removeConnectedAttribute(int linkId) { m_connectedAttributes.erase(linkId);`
- `[[nodiscard]] std::map<int, Attribute *> &getConnectedAttributes() { return m_connectedAttributes;`
- `[[nodiscard]] Node *getParentNode() const { return m_parentNode;`
- `void clearOutputData() { m_outputData.clear();`
- `[[nodiscard]] std::vector<u8>& getDefaultData() { return m_defaultData;`
- `static void setIdCounter(int id);`
- `void setParentNode(Node *node) { m_parentNode = node;`

## `libimhex/include/hex/data_processor/link.hpp`
Namespaces: `hex::dp`

Types: `class Link`

Function-like declarations:
- `Link(int from, int to);`
- `[[nodiscard]] int getId() const { return m_id;`
- `void setId(int id) { m_id = id;`
- `[[nodiscard]] int getFromId() const { return m_from;`
- `[[nodiscard]] int getToId() const { return m_to;`
- `static void setIdCounter(int id);`

## `libimhex/include/hex/data_processor/node.hpp`
Namespaces: `hex::prv`, `hex::dp`

Types: `class Provider`, `class Overlay`, `class Node`, `struct NodeError`

Function-like declarations:
- `Node(UnlocalizedString unlocalizedTitle, std::vector<Attribute> attributes);`
- `virtual ~Node() = default;`
- `[[nodiscard]] int getId() const { return m_id;`
- `void setId(int id) { m_id = id;`
- `[[nodiscard]] const UnlocalizedString &getUnlocalizedName() const { return m_unlocalizedName;`
- `void setUnlocalizedName(const UnlocalizedString &unlocalizedName) { m_unlocalizedName = unlocalizedName;`
- `[[nodiscard]] const UnlocalizedString &getUnlocalizedTitle() const { return m_unlocalizedTitle;`
- `void setUnlocalizedTitle(std::string title) { m_unlocalizedTitle = std::move(title);`
- `[[nodiscard]] std::vector<Attribute> &getAttributes() { return m_attributes;`
- `[[nodiscard]] const std::vector<Attribute> &getAttributes() const { return m_attributes;`
- `void draw();`
- `virtual void process() = 0;`
- `virtual void store(nlohmann::json &j) const { std::ignore = j;`
- `virtual void load(const nlohmann::json &j) { std::ignore = j;`
- `attribute.clearOutputData();`
- `static void setIdCounter(int id);`
- `const std::vector<u8>& getBufferOnInput(u32 index);`
- `const i128& getIntegerOnInput(u32 index);`
- `const double& getFloatOnInput(u32 index);`
- `void setBufferOnOutput(u32 index, std::span<const u8> data);`
- `void setIntegerOnOutput(u32 index, i128 integer);`
- `void setFloatOnOutput(u32 index, double floatingPoint);`
- `static void interrupt();`
- `Attribute& getAttribute(u32 index);`
- `Attribute *getConnectedInputAttribute(u32 index);`
- `void markInputProcessed(u32 index);`
- `void unmarkInputProcessed(u32 index);`
- `[[noreturn]] void throwNodeError(const std::string &message);`
- `void setOverlayData(u64 address, const std::vector<u8> &data);`
- `void setAttributes(std::vector<Attribute> attributes);`

## `libimhex/include/hex/helpers/auto_reset.hpp`
Namespaces: `hex`, `impl`

Types: `class AutoResetBase`, `class AutoReset`

Function-like declarations:
- `virtual ~AutoResetBase() = default;`
- `virtual void reset() = 0;`
- `ImHexApi::System::impl::addAutoResetObject(this);`
- `ImHexApi::System::impl::removeAutoResetObject(this);`
- `} else if constexpr (requires { m_value.clear();`

## `libimhex/include/hex/helpers/binary_pattern.hpp`
Namespaces: `hex`

Types: `class BinaryPattern`, `struct Pattern`

Function-like declarations:
- `BinaryPattern() = default;`
- `explicit BinaryPattern(const std::string &pattern);`
- `[[nodiscard]] bool isValid() const;`
- `[[nodiscard]] u64 getSize() const;`
- `[[nodiscard]] bool matches(const std::vector<u8> &bytes) const;`
- `[[nodiscard]] bool matchesByte(u8 byte, u32 offset) const;`

## `libimhex/include/hex/helpers/concepts.hpp`
Namespaces: `hex`

Types: `struct always_false`, `class ICloneable`

Function-like declarations:
- `concept has_size = sizeof(T) == Size;`
- `virtual ~ICloneable() = default;`
- `[[nodiscard]] virtual std::unique_ptr<T> clone() const = 0;`

## `libimhex/include/hex/helpers/crypto.hpp`
Namespaces: `hex::prv`, `hex::crypt`

Types: `class Provider`, `enum class AESMode`, `enum class KeyLength`

Function-like declarations:
- `void initialize();`
- `void exit();`
- `u8 crc8(prv::Provider *&data, u64 offset, size_t size, u32 polynomial, u32 init, u32 xorOut, bool reflectIn, bool reflectOut);`
- `u16 crc16(prv::Provider *&data, u64 offset, size_t size, u32 polynomial, u32 init, u32 xorOut, bool reflectIn, bool reflectOut);`
- `u32 crc32(prv::Provider *&data, u64 offset, size_t size, u32 polynomial, u32 init, u32 xorOut, bool reflectIn, bool reflectOut);`
- `std::array<u8, 16> md5(prv::Provider *&data, u64 offset, size_t size);`
- `std::array<u8, 20> sha1(prv::Provider *&data, u64 offset, size_t size);`
- `std::array<u8, 28> sha224(prv::Provider *&data, u64 offset, size_t size);`
- `std::array<u8, 32> sha256(prv::Provider *&data, u64 offset, size_t size);`
- `std::array<u8, 48> sha384(prv::Provider *&data, u64 offset, size_t size);`
- `std::array<u8, 64> sha512(prv::Provider *&data, u64 offset, size_t size);`
- `std::array<u8, 16> md5(const std::vector<u8> &data);`
- `std::array<u8, 20> sha1(const std::vector<u8> &data);`
- `std::array<u8, 28> sha224(const std::vector<u8> &data);`
- `std::array<u8, 32> sha256(const std::vector<u8> &data);`
- `std::array<u8, 48> sha384(const std::vector<u8> &data);`
- `std::array<u8, 64> sha512(const std::vector<u8> &data);`
- `std::vector<u8> decode64(const std::vector<u8> &input);`
- `std::vector<u8> encode64(const std::vector<u8> &input);`
- `std::vector<u8> decode16(const std::string &input);`
- `std::string encode16(const std::vector<u8> &input);`
- `i128 decodeSleb128(const std::vector<u8> &bytes);`
- `u128 decodeUleb128(const std::vector<u8> &bytes);`
- `std::vector<u8> encodeSleb128(i128 value);`
- `std::vector<u8> encodeUleb128(u128 value);`
- `wolv::util::Expected<std::vector<u8>, int> aesDecrypt(AESMode mode, KeyLength keyLength, const std::vector<u8> &key, std::array<u8, 8> nonce, std::array<u8, 8> iv, const std::vector<u8> &input);`

## `libimhex/include/hex/helpers/debugging.hpp`
Namespaces: `hex::trace`, `hex::dbg`, `impl`

Types: `struct StackTraceResult`

Function-like declarations:
- `hex::dbg::impl::drawDebugVariable(name, WOLV_STRINGIFY(name));`
- `bool &getDebugWindowState();`
- `ImGui::Checkbox(name.data(), &variable);`
- `ImGui::DragScalar(name.data(), ImGuiExt::getImGuiDataType<Type>(), &variable);`
- `ImGui::DragFloat2(name.data(), &variable.x);`
- `ImGui::InputText(name.data(), variable);`
- `ImGui::ColorEdit4(name.data(), &variable.Value.x, ImGuiColorEditFlags_AlphaBar);`
- `ImGui::End();`
- `bool debugModeEnabled();`
- `void setDebugModeEnabled(bool enabled);`
- `void printStackTrace(const trace::StackTraceResult &stackTrace);`

## `libimhex/include/hex/helpers/default_paths.hpp`
Namespaces: `hex::paths`, `impl`

Types: `class DefaultPath`, `class ConfigPath`, `class DataPath`, `class PluginPath`

Function-like declarations:
- `constexpr DefaultPath() = default;`
- `virtual ~DefaultPath() = default;`
- `DefaultPath(const DefaultPath&) = delete;`
- `DefaultPath(DefaultPath&&) = delete;`
- `DefaultPath& operator=(const DefaultPath&) = delete;`
- `DefaultPath& operator=(DefaultPath&&) = delete;`
- `virtual std::vector<std::fs::path> all() const = 0;`
- `virtual std::vector<std::fs::path> read() const;`
- `virtual std::vector<std::fs::path> write() const;`
- `std::vector<std::fs::path> all() const override;`
- `std::vector<std::fs::path> write() const override;`
- `std::vector<std::fs::path> getDataPaths(bool includeSystemFolders);`
- `std::vector<std::fs::path> getConfigPaths(bool includeSystemFolders);`
- `const static inline impl::ConfigPath Config("config");`
- `const static inline impl::ConfigPath Recent("recent");`
- `const static inline impl::ConfigPath Updates("updates");`
- `const static inline impl::PluginPath Libraries("lib");`
- `const static inline impl::PluginPath Plugins("plugins");`
- `const static inline impl::DataPath Patterns("patterns");`
- `const static inline impl::DataPath PatternsInclude("includes");`
- `const static inline impl::DataPath Magic("magic");`
- `const static inline impl::DataPath Yara("yara");`
- `const static inline impl::DataPath YaraAdvancedAnalysis("yara/advanced_analysis");`
- `const static inline impl::DataPath Backups("backups");`
- `const static inline impl::DataPath Resources("resources");`
- `const static inline impl::DataPath Constants("constants");`
- `const static inline impl::DataPath Encodings("encodings");`
- `const static inline impl::DataPath Logs("logs");`
- `const static inline impl::DataPath Scripts("scripts");`
- `const static inline impl::DataPath Inspectors("scripts/inspectors");`
- `const static inline impl::DataPath Themes("themes");`
- `const static inline impl::DataPath Nodes("scripts/nodes");`
- `const static inline impl::DataPath Layouts("layouts");`
- `const static inline impl::DataPath Workspaces("workspaces");`
- `const static inline impl::DataPath Disassemblers("disassemblers");`

## `libimhex/include/hex/helpers/encoding_file.hpp`
Namespaces: `hex`

Types: `class EncodingFile`, `enum class Type`

Function-like declarations:
- `EncodingFile();`
- `EncodingFile(const EncodingFile &other);`
- `EncodingFile(EncodingFile &&other) noexcept;`
- `EncodingFile(Type type, const std::fs::path &path);`
- `EncodingFile(Type type, const std::string &content);`
- `EncodingFile& operator=(const EncodingFile &other);`
- `EncodingFile& operator=(EncodingFile &&other) noexcept;`
- `[[nodiscard]] std::pair<std::string_view, size_t> getEncodingFor(std::span<const u8> buffer) const;`
- `[[nodiscard]] u64 getEncodingLengthFor(std::span<u8> buffer) const;`
- `[[nodiscard]] u64 getShortestSequence() const { return m_shortestSequence;`
- `[[nodiscard]] u64 getLongestSequence() const { return m_longestSequence;`
- `[[nodiscard]] std::string decodeAll(std::span<const u8> buffer) const;`
- `[[nodiscard]] bool valid() const { return m_valid;`
- `[[nodiscard]] const std::string& getTableContent() const { return m_tableContent;`
- `[[nodiscard]] const std::string& getName() const { return m_name;`
- `void parse(const std::string &content);`
- `u64 m_shortestSequence = std::numeric_limits<u64>::max();`
- `u64 m_longestSequence = std::numeric_limits<u64>::min();`

## `libimhex/include/hex/helpers/fmt.hpp`

## `libimhex/include/hex/helpers/fs.hpp`

Types: `enum class DialogMode`, `struct ItemFilter`

Function-like declarations:
- `void setFileBrowserErrorCallback(const std::function<void(const std::string&)> &callback);`
- `bool openFileBrowser(DialogMode mode, const std::vector<ItemFilter> &validExtensions, const std::function<void(std::fs::path)> &callback, const std::string &defaultPath = {}, bool multiple = false);`
- `void openFileExternal(std::fs::path filePath);`
- `void openFolderExternal(std::fs::path dirPath);`
- `void openFolderWithSelectionExternal(std::fs::path selectedFilePath);`
- `bool isPathWritable(const std::fs::path &path);`

## `libimhex/include/hex/helpers/http_requests.hpp`
Namespaces: `hex`

Types: `class HttpRequest`, `class ResultBase`, `class Result`

Function-like declarations:
- `ResultBase() = default;`
- `Result() = default;`
- `HttpRequest(std::string method, std::string url);`
- `~HttpRequest();`
- `HttpRequest(const HttpRequest&) = delete;`
- `HttpRequest& operator=(const HttpRequest&) = delete;`
- `HttpRequest(HttpRequest &&other) noexcept;`
- `HttpRequest& operator=(HttpRequest &&other) noexcept;`
- `static void setProxyState(bool enabled);`
- `static void setProxyUrl(std::string proxy);`
- `std::future<Result<T>> downloadFile(const std::fs::path &path);`
- `std::future<Result<std::vector<u8>>> downloadFile();`
- `std::future<Result<T>> uploadFile(const std::fs::path &path, const std::string &mimeName = "filename");`
- `std::future<Result<T>> uploadFile(std::vector<u8> data, const std::string &mimeName = "filename", const std::fs::path &fileName = "data.bin");`
- `std::future<Result<T>> execute();`
- `static std::string urlEncode(const std::string &input);`
- `static std::string urlDecode(const std::string &input);`
- `void setProgress(float progress) { m_progress = progress;`
- `bool isCanceled() const { return m_canceled;`
- `static size_t writeToVector(void *contents, size_t size, size_t nmemb, void *userdata);`
- `static size_t writeToFile(void *contents, size_t size, size_t nmemb, void *userdata);`
- `Result<T> executeImpl(std::vector<u8> &data);`
- `static void checkProxyErrors();`
- `void setDefaultConfig();`

## `libimhex/include/hex/helpers/http_requests_emscripten.hpp`
Namespaces: `hex`

Function-like declarations:
- `auto result = this->executeImpl<T>(response);`
- `wolv::io::File file(path, wolv::io::File::Mode::Create);`
- `file.writeBuffer(reinterpret_cast<const u8*>(result.getData().data()), result.getData().size());`
- `throw std::logic_error("Not implemented");`
- `strcpy(m_attr.requestMethod, m_method.c_str());`
- `headers.push_back(it->first.c_str());`
- `headers.push_back(it->second.c_str());`
- `headers.push_back(nullptr);`
- `emscripten_fetch_t* fetch = emscripten_fetch(&m_attr, m_url.c_str());`
- `data.resize(fetch->numBytes);`
- `std::copy(fetch->data, fetch->data + fetch->numBytes, data.begin());`

## `libimhex/include/hex/helpers/http_requests_native.hpp`
Namespaces: `hex`, `impl`

Function-like declarations:
- `void setWriteFunctions(CURL *curl, wolv::io::File &file);`
- `void setWriteFunctions(CURL *curl, std::vector<u8> &data);`
- `void setupFileUpload(CURL *curl, wolv::io::File &file, const std::string &fileName, const std::string &mimeName);`
- `void setupFileUpload(CURL *curl, const std::vector<u8> &data, const std::fs::path &fileName, const std::string &mimeName);`
- `int executeCurl(CURL *curl, const std::string &url, const std::string &method, const std::string &body, std::map<std::string, std::string> &headers);`
- `long getStatusCode(CURL *curl);`
- `std::string getStatusText(int result);`
- `wolv::io::File file(path, wolv::io::File::Mode::Create);`
- `impl::setWriteFunctions(m_curl, file);`
- `auto fileName = wolv::util::toUTF8String(path.filename());`
- `wolv::io::File file(path, wolv::io::File::Mode::Read);`
- `impl::setupFileUpload(m_curl, file, fileName, mimeName);`
- `impl::setWriteFunctions(m_curl, responseData);`
- `impl::setupFileUpload(m_curl, data, fileName, mimeName);`
- `setDefaultConfig();`
- `std::scoped_lock lock(m_transmissionMutex);`
- `log::error("Http request '{0} {1}' failed with error {2}: '{3}'", m_method, m_url, u32(result), impl::getStatusText(result));`
- `checkProxyErrors();`

## `libimhex/include/hex/helpers/keys.hpp`

Types: `enum class Keys`, `enum Keys`

Function-like declarations:
- `enum Keys scanCodeToKey(int scanCode);`
- `int keyToScanCode(enum Keys key);`

## `libimhex/include/hex/helpers/literals.hpp`
Namespaces: `hex::literals`

## `libimhex/include/hex/helpers/logger.hpp`
Namespaces: `impl`, `color`

Types: `struct LogEntry`

Function-like declarations:
- `[[nodiscard]] FILE *getDestination();`
- `[[nodiscard]] wolv::io::File& getFile();`
- `[[nodiscard]] bool isRedirected();`
- `[[maybe_unused]] void redirectToFile();`
- `[[maybe_unused]] void enableColorPrinting();`
- `[[nodiscard]] bool isLoggingSuspended();`
- `[[nodiscard]] bool isDebugLoggingEnabled();`
- `void lockLoggerMutex();`
- `void unlockLoggerMutex();`
- `const std::vector<LogEntry>& getLogEntries();`
- `void addLogEntry(std::string_view project, std::string_view level, std::string message);`
- `[[maybe_unused]] void printPrefix(FILE *dest, fmt::text_style ts, std::string_view level, std::string_view projectName);`
- `lockLoggerMutex();`
- `ON_SCOPE_EXIT { unlockLoggerMutex();`
- `auto dest = getDestination();`
- `printPrefix(dest, ts, level, IMHEX_PROJECT_NAME);`
- `auto message = fmt::format(fmt, std::forward<Args>(args)...);`
- `fmt::print(dest, "{}\n", message);`
- `std::fflush(dest);`
- `addLogEntry(IMHEX_PROJECT_NAME, level, std::move(message));`
- `fmt::color debug();`
- `fmt::color info();`
- `fmt::color warn();`
- `fmt::color error();`
- `fmt::color fatal();`
- `void suspendLogging();`
- `void resumeLogging();`
- `void enableDebugLogging();`
- `impl::print(fg(impl::color::debug()) | fmt::emphasis::bold, "[DEBUG]", fmt, std::forward<Args>(args)...);`
- `impl::addLogEntry(IMHEX_PROJECT_NAME, "[DEBUG]", fmt::format(fmt, std::forward<Args>(args)...));`
- `impl::print(fg(impl::color::info()) | fmt::emphasis::bold, "[INFO] ", fmt, std::forward<Args>(args)...);`
- `impl::print(fg(impl::color::warn()) | fmt::emphasis::bold, "[WARN] ", fmt, std::forward<Args>(args)...);`
- `impl::print(fg(impl::color::error()) | fmt::emphasis::bold, "[ERROR]", fmt, std::forward<Args>(args)...);`
- `impl::print(fg(impl::color::fatal()) | fmt::emphasis::bold, "[FATAL]", fmt, std::forward<Args>(args)...);`
- `impl::lockLoggerMutex();`
- `ON_SCOPE_EXIT { impl::unlockLoggerMutex();`
- `auto dest = impl::getDestination();`
- `fmt::print(dest, fmt, std::forward<Args>(args)...);`
- `fmt::print("\n");`

## `libimhex/include/hex/helpers/magic.hpp`
Namespaces: `hex::prv`, `hex::magic`

Types: `class Provider`, `struct FoundPattern`

Function-like declarations:
- `bool compile();`
- `std::string getDescription(const std::vector<u8> &data, bool firstEntryOnly = false);`
- `std::string getDescription(prv::Provider *provider, u64 address = 0x00, size_t size = 100_KiB, bool firstEntryOnly = false);`
- `std::string getMIMEType(const std::vector<u8> &data, bool firstEntryOnly = false);`
- `std::string getMIMEType(prv::Provider *provider, u64 address = 0x00, size_t size = 100_KiB, bool firstEntryOnly = false);`
- `std::string getExtensions(const std::vector<u8> &data, bool firstEntryOnly = false);`
- `std::string getExtensions(prv::Provider *provider, u64 address = 0x00, size_t size = 100_KiB, bool firstEntryOnly = false);`
- `std::string getAppleCreatorType(const std::vector<u8> &data, bool firstEntryOnly = false);`
- `std::string getAppleCreatorType(prv::Provider *provider, u64 address = 0x00, size_t size = 100_KiB, bool firstEntryOnly = false);`
- `bool isValidMIMEType(const std::string &mimeType);`
- `std::vector<FoundPattern> findViablePatterns(prv::Provider *provider, Task* task = nullptr);`

## `libimhex/include/hex/helpers/menu_items.hpp`
Namespaces: `hex::menu`

Function-like declarations:
- `void enableNativeMenuBar(bool enabled);`
- `bool isNativeMenuBarUsed();`
- `bool beginMainMenuBar();`
- `void endMainMenuBar();`
- `bool beginMenu(const char *label, bool enabled = true);`
- `void endMenu();`
- `bool beginMenuEx(const char* label, const char* icon, bool enabled = true);`
- `bool menuItem(const char *label, const Shortcut &shortcut = Shortcut::None, bool selected = false, bool enabled = true);`
- `bool menuItem(const char *label, const Shortcut &shortcut, bool *selected, bool enabled = true);`
- `bool menuItemEx(const char *label, const char *icon, const Shortcut &shortcut = Shortcut::None, bool selected = false, bool enabled = true);`
- `bool menuItemEx(const char *label, const char *icon, const Shortcut &shortcut, bool *selected, bool enabled = true);`
- `void menuSeparator();`

## `libimhex/include/hex/helpers/opengl.hpp`
Namespaces: `hex::gl`, `impl`

Types: `class Vector`, `class Matrix`, `enum RotationSequence`, `enum class MatrixElements`, `class Shader`, `enum class BufferType`, `class Buffer`, `class VertexArray`, `class Texture`, `class FrameBuffer`, `class AxesVectors`, `class AxesBuffers`, `class GridVectors`, `class GridBuffers`, `class LightSourceVectors`, `class LightSourceBuffers`

Function-like declarations:
- `Vector() = default;`
- `T &operator[](size_t index) { return m_data[index];`
- `const T &operator[](size_t index) const { return m_data[index];`
- `std::array<T, Size> &asArray() { return m_data;`
- `T *data() { return m_data.data();`
- `const T *data() const { return m_data.data();`
- `[[nodiscard]] size_t size() const { return m_data.size();`
- `auto length = copy.magnitude();`
- `T *data() { return this->mat.data();`
- `const T *data() const { return this->mat.data();`
- `mat[i*Columns+j] = A(i, j);`
- `Matrix result(0.0);`
- `result(i, j) = this->mat[i * Columns + j] + A(i, j);`
- `result(i, j) = this->mat[i * Columns + j] - A(i, j);`
- `Matrix I(0);`
- `if(i == j) I.updateElement(i, j, 1);`
- `Matrix t(0);`
- `t.updateElement(i, j, this->mat[j * Rows + i]);`
- `Matrix<T, Rows, Columns> result(0.0);`
- `result(i, j) += A(i,k) * B(k, j);`
- `Matrix<T, Rows, Columns> result(0);`
- `result.updateElement(i, j, a[i] * b[j]);`
- `Vector<T, Rows> result(0);`
- `result[i] += A(i, j) * b[j];`
- `Vector<T, Columns> result(0);`
- `result[j] += b[i] * A(i, j);`
- `Matrix<T,4,4> rotation(0);`
- `Sx = -sin(angles[0]);`
- `Sy = -sin(angles[1]);`
- `Sz = -sin(angles[2]);`
- `rotation.updateElement(0, 0, Cz * Cy - Sz * Sx * Sy);`
- `rotation.updateElement(0, 1, -Sz * Cx);`
- `rotation.updateElement(0, 2, Cz * Sy + Sz * Sx * Cy);`
- `rotation.updateElement(1, 0, Sz * Cy + Cz * Sx * Sy);`
- `rotation.updateElement(1, 1, Cz * Cx);`
- `rotation.updateElement(1, 2, Sz * Sy - Cz * Sx * Cy);`
- `rotation.updateElement(2, 0, -Cx * Sy);`
- `rotation.updateElement(2, 1, Sx);`
- `rotation.updateElement(2, 2, Cx * Cy);`
- `rotation.updateElement(0, 0, Cz * Cy);`
- `rotation.updateElement(0, 1, Sx * Sy * Cz - Sz * Cx);`
- `rotation.updateElement(0, 2, Sz * Sx + Cz * Sy * Cx);`
- `rotation.updateElement(1, 0, Sz * Cy);`
- `rotation.updateElement(1, 1, Sz * Sy * Sx + Cz * Cx);`
- `rotation.updateElement(1, 2, Sz * Sy * Cx - Cz * Sx);`
- `rotation.updateElement(2, 0, -Sy);`
- `rotation.updateElement(2, 1, Cy * Sx);`
- `rotation.updateElement(2, 2, Cy*Cx);`
- `rotation.updateElement(0, 0, Cy * Cz);`
- `rotation.updateElement(0, 1, -Cy * Sz);`
- `rotation.updateElement(0, 2, Sy);`
- `rotation.updateElement(1, 0, Sx * Sy * Cz + Cx * Sz);`
- `rotation.updateElement(1, 1, -Sx * Sy * Sz + Cx * Cz);`
- `rotation.updateElement(1, 2, -Sx * Cy);`
- `rotation.updateElement(2, 0, -Cx * Sy * Cz + Sx * Sz);`
- `rotation.updateElement(2, 1, Cx * Sy * Sz + Sx * Cz);`
- `rotation.updateElement(0, 1, -Sz);`
- `rotation.updateElement(0, 2, Cz * Sy);`
- `rotation.updateElement(1, 0, Cx * Cy * Sz + Sx * Sy);`
- `rotation.updateElement(1, 1, Cx * Cz);`
- `rotation.updateElement(1, 2, Cx * Sy * Sz - Sx * Cy);`
- `rotation.updateElement(2, 0, Sx * Cy * Sz - Cx * Sy);`
- `rotation.updateElement(2, 1, Sx * Cz);`
- `rotation.updateElement(2, 2, Sx * Sy * Sz + Cx * Cy);`
- `rotation.updateElement(0, 0, Cy*Cz+Sy*Sx*Sz );`
- `rotation.updateElement(0, 1, Cz*Sy*Sx-Cy*Sz);`
- `rotation.updateElement(0, 2, Sy*Cx);`
- `rotation.updateElement(1, 0, Cx*Sz);`
- `rotation.updateElement(1, 1, Cx*Cz);`
- `rotation.updateElement(1, 2, -Sx);`
- `rotation.updateElement(2, 0, Cy*Sx*Sz-Cz*Sy);`
- `rotation.updateElement(2, 1, Cy*Cz*Sx+Sy*Sz);`
- `rotation.updateElement(0, 0, Cy*Cz);`
- `rotation.updateElement(0, 1, Sy*Sx-Cy*Cx*Sz);`
- `rotation.updateElement(0, 2, Cx*Sy+Cy*Sz*Sx);`
- `rotation.updateElement(1, 0, Sz);`
- `rotation.updateElement(1, 1, Cz*Cx);`
- `rotation.updateElement(1, 2, -Cz*Sx);`
- `rotation.updateElement(2, 0, -Cz*Sy);`
- `rotation.updateElement(2, 1, Cy*Sx+Cx*Sy*Sz);`
- `rotation.updateElement(2, 2, Cy*Cx-Sy*Sz*Sx);`
- `rotation.updateElement(3, 3, 1);`
- `T theta = rotationVector3.magnitude();`
- `axis = axis.normalize();`
- `Matrix<T,4,4> rotation = Matrix<T,4,4>::identity();`
- `T S = sin(theta);`
- `T C = cos(theta);`
- `rotation.updateElement(0, 0, C + a00);`
- `rotation.updateElement(0, 1, a01 - a2S);`
- `rotation.updateElement(0, 2, a02 + a1S);`
- `rotation.updateElement(1, 0, a10 + a2S);`
- `rotation.updateElement(1, 1, C + a11);`
- `rotation.updateElement(1, 2, a12 - a0S);`
- `rotation.updateElement(2, 0, a20 - a1S);`
- `rotation.updateElement(2, 1, a21 + a0S);`
- `rotation.updateElement(2, 2, C + a22);`
- `Sx = sin(angles[0]);`
- `Sy = sin(angles[1]);`
- `Sz = sin(angles[2]);`
- `Matrix<T,4,4> transform( 0);`
- `Matrix<T,3,3> rotation = getRotationMatrix(ypr, radians);`
- `for(int i=0;`
- `for(int j=0;`
- `transform.updateElement(i, j, rotation.getElement(i, j));`
- `transform.updateElement(0,3, xyz[0]);`
- `transform.updateElement(1,3, xyz[1]);`
- `transform.updateElement(2,3, xyz[2]);`
- `transform.updateElement(3,3, 1);`
- `xyz.push_back(transform_matrix.getElement(0,3));`
- `xyz.push_back(transform_matrix.getElement(1,3));`
- `xyz.push_back(transform_matrix.getElement(2,3));`
- `Matrix<T,3,3> rotation(0);`
- `rotation.updateElement(i, j, transform_matrix.getElement(i, j));`
- `T sy = sqrt(rotation.getElement(0,0) * rotation.getElement(0,0) + rotation.getElement(1,0) * rotation.getElement(1,0) );`
- `x = atan2(rotation.getElement(1,0), rotation.getElement(0,0));`
- `y = atan2(-rotation.getElement(2,0), sy);`
- `z = atan2(rotation.getElement(2,1), rotation.getElement(2,2));`
- `z = atan2(-rotation.getElement(1,2), rotation.getElement(1,1));`
- `result.push_back(x);`
- `result.push_back(y);`
- `result.push_back(z);`
- `Matrix<float,4,4> GetPerspectiveMatrix( float viewWidth, float viewHeight, float nearVal, float farVal, bool actionType = false);`
- `Matrix<float,4,4> GetOrthographicMatrix( float viewWidth, float viewHeight, float nearVal, float farVal, bool actionType = false);`
- `Matrix<T,4,4> result(0);`
- `result.updateElement(0,0,sign * nearVal/width);`
- `result.updateElement(1,1, sign * nearVal/height);`
- `result.updateElement(2,2,sign * (farVal + nearVal)/( farVal - nearVal ));`
- `result.updateElement(3,2,sign * 2*farVal * nearVal/( farVal - nearVal ));`
- `result.updateElement(2,3,-sign);`
- `Shader() = default;`
- `Shader(std::string_view vertexSource, std::string_view fragmentSource);`
- `~Shader();`
- `Shader(const Shader&) = delete;`
- `Shader(Shader&& other) noexcept;`
- `Shader& operator=(const Shader&) = delete;`
- `Shader& operator=(Shader&& other) noexcept;`
- `void bind() const;`
- `void unbind() const;`
- `bool isValid() const { return m_program != 0;`
- `void setUniform(std::string_view name, const int &value);`
- `void setUniform(std::string_view name, const float &value);`
- `bool hasUniform(std::string_view name);`
- `glUniform2f(getUniformLocation(name), value[0], value[1]);`
- `else if constexpr (N == 3) glUniform3f(getUniformLocation(name), value[0], value[1], value[2]);`
- `else if constexpr (N == 4) glUniform4f(getUniformLocation(name), value[0], value[1], value[2],value[3]);`
- `glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, value.data());`
- `void compile(GLuint shader, std::string_view source) const;`
- `GLint getUniformLocation(std::string_view name);`
- `Buffer() = default;`
- `Buffer(BufferType type, std::span<const T> data);`
- `~Buffer();`
- `Buffer(const Buffer&) = delete;`
- `Buffer(Buffer&& other) noexcept;`
- `Buffer& operator=(const Buffer&) = delete;`
- `Buffer& operator=(Buffer&& other) noexcept;`
- `void draw(unsigned primitive) const;`
- `size_t getSize() const;`
- `void update(std::span<const T> data);`
- `VertexArray();`
- `~VertexArray();`
- `VertexArray(const VertexArray&) = delete;`
- `VertexArray(VertexArray&& other) noexcept;`
- `VertexArray& operator=(const VertexArray&) = delete;`
- `VertexArray& operator=(VertexArray&& other) noexcept;`
- `glEnableVertexAttribArray(index);`
- `buffer.bind();`
- `glVertexAttribPointer(index, size, gl::impl::getType<T>(), GL_FALSE, size * sizeof(T), nullptr);`
- `buffer.unbind();`
- `Texture(u32 width, u32 height);`
- `~Texture();`
- `Texture(const Texture&) = delete;`
- `Texture(Texture&& other) noexcept;`
- `Texture& operator=(const Texture&) = delete;`
- `Texture& operator=(Texture&& other) noexcept;`
- `GLuint getTexture() const;`
- `u32 getWidth() const;`
- `u32 getHeight() const;`
- `GLuint release();`
- `FrameBuffer(u32 width, u32 height);`
- `~FrameBuffer();`
- `FrameBuffer(const FrameBuffer&) = delete;`
- `FrameBuffer(FrameBuffer&& other) noexcept;`
- `FrameBuffer& operator=(const FrameBuffer&) = delete;`
- `FrameBuffer& operator=(FrameBuffer&& other) noexcept;`
- `void attachTexture(const Texture &texture) const;`
- `AxesVectors();`
- `AxesBuffers(const VertexArray& axesVertexArray, const AxesVectors &axesVectors);`
- `GridVectors(int sliceCount);`
- `GridBuffers(const VertexArray &gridVertexArray, const GridVectors &gridVectors);`
- `LightSourceVectors(int res);`
- `void moveTo(const Vector<float, 3> &position);`
- `LightSourceBuffers(const VertexArray &sourceVertexArray, const LightSourceVectors &sourceVectors);`
- `void moveVertices(const VertexArray &sourceVertexArray, const LightSourceVectors& sourceVectors);`
- `void updateColors(const VertexArray& sourceVertexArray, const LightSourceVectors& sourceVectors);`

## `libimhex/include/hex/helpers/patches.hpp`
Namespaces: `hex`, `prv`

Types: `class Provider`, `enum class IPSError`, `enum class PatchKind`, `class Patches`

Function-like declarations:
- `Patches() = default;`
- `static wolv::util::Expected<Patches, IPSError> fromProvider(hex::prv::Provider *provider);`
- `static wolv::util::Expected<Patches, IPSError> fromIPSPatch(const std::vector<u8> &ipsPatch);`
- `static wolv::util::Expected<Patches, IPSError> fromIPS32Patch(const std::vector<u8> &ipsPatch);`
- `wolv::util::Expected<std::vector<u8>, IPSError> toIPSPatch() const;`
- `wolv::util::Expected<std::vector<u8>, IPSError> toIPS32Patch() const;`
- `const auto& get() const { return m_patches;`
- `auto& get() { return m_patches;`

## `libimhex/include/hex/helpers/scaling.hpp`
Namespaces: `hex`

Function-like declarations:
- `[[nodiscard]] float operator""_scaled(long double value);`
- `[[nodiscard]] float operator""_scaled(unsigned long long value);`
- `[[nodiscard]] ImVec2 scaled(const ImVec2 &vector);`
- `[[nodiscard]] ImVec2 scaled(float x, float y);`

## `libimhex/include/hex/helpers/semantic_version.hpp`

Types: `class SemanticVersion`

Function-like declarations:
- `SemanticVersion() = default;`
- `SemanticVersion(u32 major, u32 minor, u32 patch);`
- `SemanticVersion(std::string version);`
- `SemanticVersion(std::string_view version);`
- `SemanticVersion(const char *version);`
- `std::strong_ordering operator<=>(const SemanticVersion &) const;`
- `bool operator==(const SemanticVersion &other) const;`
- `u32 major() const;`
- `u32 minor() const;`
- `u32 patch() const;`
- `bool nightly() const;`
- `const std::string& buildType() const;`
- `bool isValid() const;`
- `std::string get(bool withBuildType = true) const;`

## `libimhex/include/hex/helpers/tar.hpp`
Namespaces: `hex`

Types: `struct mtar_t`, `class Tar`, `enum class Mode`

Function-like declarations:
- `Tar() = default;`
- `Tar(const std::fs::path &path, Mode mode);`
- `~Tar();`
- `Tar(const Tar&) = delete;`
- `Tar(Tar&&) noexcept;`
- `Tar &operator=(Tar &&other) noexcept;`
- `void close();`
- `std::string getOpenErrorString() const;`
- `[[nodiscard]] std::vector<u8> readVector(const std::fs::path &path) const;`
- `[[nodiscard]] std::string readString(const std::fs::path &path) const;`
- `void writeVector(const std::fs::path &path, const std::vector<u8> &data) const;`
- `void writeString(const std::fs::path &path, const std::string &data) const;`
- `[[nodiscard]] std::vector<std::fs::path> listEntries(const std::fs::path &basePath = "/") const;`
- `[[nodiscard]] bool contains(const std::fs::path &path) const;`
- `void extract(const std::fs::path &path, const std::fs::path &outputPath) const;`
- `void extractAll(const std::fs::path &outputPath) const;`
- `[[nodiscard]] bool isValid() const { return m_valid;`

## `libimhex/include/hex/helpers/types.hpp`
Namespaces: `hex`

Types: `struct Region`, `struct NonNull`

Function-like declarations:
- `[[nodiscard]] constexpr u64 getStartAddress() const { return this->address;`
- `[[nodiscard]] constexpr size_t getSize() const { return this->size;`
- `NonNull(std::nullptr_t) = delete;`
- `NonNull(std::integral auto) = delete;`
- `NonNull(bool) = delete;`
- `[[nodiscard]] T get() const { return pointer;`
- `[[nodiscard]] T operator->() const { return pointer;`
- `[[nodiscard]] std::remove_pointer_t<T> operator*() const { return *pointer;`
- `[[nodiscard]] operator T() const { return pointer;`

## `libimhex/include/hex/helpers/udp_server.hpp`
Namespaces: `hex`

Types: `class UDPServer`

Function-like declarations:
- `UDPServer() = default;`
- `UDPServer(u16 port, Callback callback);`
- `~UDPServer();`
- `UDPServer(const UDPServer&) = delete;`
- `UDPServer& operator=(const UDPServer&) = delete;`
- `void start();`
- `void stop();`
- `[[nodiscard]] u16 getPort() const { return m_port;`
- `void run();`

## `libimhex/include/hex/helpers/utils.hpp`
Namespaces: `hex`, `prv`

Types: `class Provider`, `struct SizeTypeImpl`

Function-like declarations:
- `size_t signalLength = std::max<double>(1.0, double(data.size()) / channels);`
- `size_t stride = std::max(1.0, double(signalLength) / count);`
- `result.resize(channels);`
- `result[i].reserve(count);`
- `result.reserve(count);`
- `result[j].push_back(data[i + j]);`
- `size_t stride = std::max(1.0, double(data.size()) / count);`
- `result.push_back(data[i]);`
- `std::copy(lhs.begin(), lhs.end(), std::back_inserter(result));`
- `std::copy(rhs.begin(), rhs.end(), std::back_inserter(result));`
- `[[nodiscard]] std::string to_string(u128 value);`
- `[[nodiscard]] std::string to_string(i128 value);`
- `[[nodiscard]] std::string toLower(std::string string);`
- `[[nodiscard]] std::string toUpper(std::string string);`
- `[[nodiscard]] std::vector<u8> parseHexString(std::string string);`
- `[[nodiscard]] std::optional<u8> parseBinaryString(const std::string &string);`
- `[[nodiscard]] std::string toByteString(u64 bytes);`
- `[[nodiscard]] std::string makePrintable(u8 c);`
- `void startProgram(const std::vector<std::string> &command);`
- `int executeCommand(const std::string &command);`
- `std::optional<std::string> executeCommandWithOutput(const std::string &command);`
- `void openWebpage(std::string url);`
- `extern "C" void registerFont(const char *fontName, const char *fontPath);`
- `const std::map<std::fs::path, std::string>& getFonts();`
- `[[nodiscard]] std::string encodeByteString(const std::vector<u8> &bytes);`
- `[[nodiscard]] std::vector<u8> decodeByteString(const std::string &string);`
- `[[nodiscard]] std::wstring utf8ToUtf16(const std::string& utf8);`
- `[[nodiscard]] std::string utf16ToUtf8(const std::wstring& utf16);`
- `ValueType mask = (std::numeric_limits<ValueType>::max() >> (((sizeof(value) * 8) - 1) - (from - to))) << to;`
- `std::memcpy(&value, &bytes[index], std::min(sizeof(value), bytes.size() - index));`
- `u64 mask = (std::numeric_limits<u64>::max() >> (64 - (from + 1)));`
- `i128 mask = 1ULL << (numBits - 1);`
- `result |= (value & (1 << bit)) != 0;`
- `size = std::min(size, sizeof(T));`
- `std::array<uint8_t, sizeof(T)> data = { 0 };`
- `std::memcpy(&data[0], &value, size);`
- `std::swap(data[i], data[size - 1 - i]);`
- `std::memcpy(&result, &data[0], size);`
- `buffer.push_back(std::move(first));`
- `moveToVector(buffer, std::move(rest)...);`
- `moveToVector(result, T(std::move(first)), std::move(rest)...);`
- `[[nodiscard]] std::string toEngineeringString(double value);`
- `auto byteString = std::string(string);`
- `std::erase(byteString, ' ');`
- `auto value = wolv::util::from_chars<u64>(byteString.substr(i, 2), 16);`
- `result.push_back(*value);`
- `result += (number & (0b1LLU << bit)) == 0 ? '0' : '1';`
- `const u32 sign = value >> (ExponentBits + MantissaBits);`
- `const u32 exponent = (value >> MantissaBits) & ((1u << ExponentBits) - 1);`
- `u32 mantissa = value & ((1u << MantissaBits) - 1);`
- `i32 inputBias = (1 << (ExponentBits - 1)) - 1;`
- `mantissa &= ((1u << MantissaBits) - 1);`
- `result = (sign << 31) | (adjustedExp << 23) | (mantissa << (23 - MantissaBits));`
- `result = (sign << 31) | (0xFF << 23) | (mantissa << (23 - MantissaBits));`
- `std::memcpy(&floatResult, &result, sizeof(float));`
- `auto iter = std::search(a.begin(), a.end(), b.begin(), b.end(), [](char ch1, char ch2) { });`
- `const T *value = std::get_if<T>(&variant);`
- `[[nodiscard]] std::optional<u8> hexCharToValue(char c);`
- `[[nodiscard]] bool isProcessElevated();`
- `[[nodiscard]] std::optional<std::string> getEnvironmentVariable(const std::string &env);`
- `[[nodiscard]] std::string limitStringLength(const std::string &string, size_t maxLength, bool fromBothEnds = true);`
- `[[nodiscard]] std::optional<std::fs::path> getInitialFilePath();`
- `[[nodiscard]] std::string generateHexView(u64 offset, u64 size, prv::Provider *provider);`
- `[[nodiscard]] std::string generateHexView(u64 offset, const std::vector<u8> &data);`
- `[[nodiscard]] std::string formatSystemError(i32 error);`
- `[[nodiscard]] void* getContainingModule(void* symbol);`
- `[[nodiscard]] std::optional<ImColor> blendColors(const std::optional<ImColor> &a, const std::optional<ImColor> &b);`
- `std::optional<std::chrono::system_clock::time_point> parseTime(std::string_view format, const std::string &timeString);`
- `std::optional<std::string> getOSLanguage();`
- `void showErrorMessageBox(const std::string &message);`
- `void showToastMessage(const std::string &title, const std::string &message);`

## `libimhex/include/hex/helpers/utils_linux.hpp`
Namespaces: `hex`

Function-like declarations:
- `void executeCmd(const std::vector<std::string> &argsVector);`

## `libimhex/include/hex/helpers/utils_macos.hpp`

Types: `struct GLFWwindow`

Function-like declarations:
- `void errorMessageMacos(const char *message);`
- `void openWebpageMacos(const char *url);`
- `bool isMacosSystemDarkModeEnabled();`
- `bool isMacosFullScreenModeEnabled(GLFWwindow *window);`
- `float getBackingScaleFactor();`
- `void setupMacosWindowStyle(GLFWwindow *window, bool borderlessWindowMode);`
- `void enumerateFontsMacos();`
- `void macosHandleTitlebarDoubleClickGesture(GLFWwindow *window);`
- `void macosSetWindowMovable(GLFWwindow *window, bool movable);`
- `bool macosIsWindowBeingResizedByUser(GLFWwindow *window);`
- `void macosMarkContentEdited(GLFWwindow *window, bool edited = true);`
- `void macosGetKey(Keys key, int *output);`
- `bool macosIsMainInstance();`
- `void macosSendMessageToMainInstance(const unsigned char *data, size_t size);`
- `void macosInstallEventListener();`
- `void toastMessageMacos(const char *title, const char *message);`

## `libimhex/include/hex/plugin.hpp`

Types: `struct PluginFunctionHelperInstantiation`, `struct PluginFeatureFunctionHelper`, `struct PluginSubCommandsFunctionHelper`

Function-like declarations:
- `static void* getFeatures();`
- `static void* getSubCommands();`
- `static auto initFeatures = [] { getFeaturesImpl() = std::vector<hex::Feature>({ IMHEX_PLUGIN_FEATURES_CONTENT });`
- `IMHEX_PLUGIN_SETUP_IMPL(name, author, description, nullptr) IMHEX_LIBRARY_SETUP_IMPL(name) IMHEX_PLUGIN_VISIBILITY_PREFIX bool isBuiltinPlugin() { return true;`
- `IMHEX_PLUGIN_SETUP_IMPL(name, author, description, isBuiltinPlugin) IMHEX_PLUGIN_VISIBILITY_PREFIX void WOLV_TOKEN_CONCAT(initializeLibrary_, IMHEX_PLUGIN_NAME)();`
- `IMHEX_PLUGIN_VISIBILITY_PREFIX const char *WOLV_TOKEN_CONCAT(getLibraryName_, IMHEX_PLUGIN_NAME)() { return name;`
- `ImGui::SetCurrentContext(ctx);`
- `hex::PluginManager::addPlugin(name, hex::PluginFunctions { \ nullptr, \ WOLV_TOKEN_CONCAT(initializeLibrary_, IMHEX_PLUGIN_NAME), \ nullptr, \ WOLV_TOKEN_CONCAT(getLibraryName_, IMHEX_PLUGIN_NAME), \ nullptr, \ nullptr, \ nullptr, \ WOLV_TOKEN_CONCAT(setImG…`
- `IMHEX_PLUGIN_VISIBILITY_PREFIX void WOLV_TOKEN_CONCAT(initializeLibrary_, IMHEX_PLUGIN_NAME)() IMHEX_PLUGIN_VISIBILITY_PREFIX const char *getPluginName() { return name;`
- `IMHEX_PLUGIN_VISIBILITY_PREFIX const char *getPluginAuthor() { return author;`
- `IMHEX_PLUGIN_VISIBILITY_PREFIX const char *getPluginDescription() { return description;`
- `IMHEX_PLUGIN_VISIBILITY_PREFIX const char *getCompatibleVersion() { return IMHEX_VERSION;`
- `IMHEX_DEFINE_PLUGIN_FEATURES();`
- `IMHEX_PLUGIN_VISIBILITY_PREFIX void initializePlugin();`
- `hex::PluginManager::addPlugin(name, hex::PluginFunctions { \ initializePlugin, \ nullptr, \ getPluginName, \ nullptr, \ getPluginAuthor, \ getPluginDescription, \ getCompatibleVersion, \ setImGuiContext, \ nullptr, \ getSubCommands, \ getFeatures, \ builtin…`
- `IMHEX_PLUGIN_VISIBILITY_PREFIX void initializePlugin() /** * This macro is used to define subcommands defined by the plugin * A subcommand consists of a key, a description, and a callback * The key is what the first argument to ImHex should be, prefixed by …`

## `libimhex/include/hex/providers/buffered_reader.hpp`
Namespaces: `hex::prv`

Types: `class ProviderReader`

Function-like declarations:
- `provider->read(address, buffer, size);`

## `libimhex/include/hex/providers/cached_provider.hpp`
Namespaces: `hex::prv`

Types: `class CachedProvider`, `struct Block`

Function-like declarations:
- `CachedProvider(size_t cacheBlockSize = 4096, size_t maxBlocks = 1024);`
- `~CachedProvider() override;`
- `OpenResult open() override;`
- `void close() override;`
- `void readRaw(u64 offset, void *buffer, size_t size) override;`
- `void writeRaw(u64 offset, const void *buffer, size_t size) override;`
- `void resizeRaw(u64 newSize) override;`
- `u64 getActualSize() const override;`
- `virtual void readFromSource(uint64_t offset, void* buffer, size_t size) = 0;`
- `virtual void writeToSource(uint64_t offset, const void* buffer, size_t size) = 0;`
- `virtual void resizeSource(uint64_t newSize) { std::ignore = newSize;`
- `virtual u64 getSourceSize() const = 0;`
- `void clearCache();`
- `constexpr u64 calcBlockIndex(u64 offset) const { return offset / m_cacheBlockSize;`
- `constexpr size_t calcBlockOffset(u64 offset) const { return offset % m_cacheBlockSize;`
- `void evictIfNeeded();`

## `libimhex/include/hex/providers/memory_provider.hpp`
Namespaces: `hex::prv`

Types: `class MemoryProvider`

Function-like declarations:
- `MemoryProvider() = default;`
- `~MemoryProvider() override = default;`
- `MemoryProvider(const MemoryProvider&) = delete;`
- `MemoryProvider& operator=(const MemoryProvider&) = delete;`
- `MemoryProvider(MemoryProvider &&provider) noexcept = default;`
- `MemoryProvider& operator=(MemoryProvider &&provider) noexcept = default;`
- `[[nodiscard]] bool isAvailable() const override { return true;`
- `[[nodiscard]] bool isReadable() const override { return true;`
- `[[nodiscard]] bool isWritable() const override { return true;`
- `[[nodiscard]] bool isResizable() const override { return true;`
- `[[nodiscard]] bool isSavable() const override { return m_name.empty();`
- `[[nodiscard]] bool isSavableAsRecent() const override { return false;`
- `[[nodiscard]] OpenResult open() override;`
- `void readRaw(u64 offset, void *buffer, size_t size) override;`
- `void writeRaw(u64 offset, const void *buffer, size_t size) override;`
- `[[nodiscard]] u64 getActualSize() const override { return m_data.size();`
- `void resizeRaw(u64 newSize) override;`
- `[[nodiscard]] std::string getName() const override { return m_name;`
- `[[nodiscard]] UnlocalizedString getTypeName() const override { return "MemoryProvider";`
- `void renameFile();`

## `libimhex/include/hex/providers/overlay.hpp`
Namespaces: `hex::prv`

Types: `class Overlay`

Function-like declarations:
- `Overlay() = default;`
- `void setAddress(u64 address) { m_address = address;`
- `[[nodiscard]] u64 getAddress() const { return m_address;`
- `[[nodiscard]] u64 getSize() const { return m_data.size();`
- `[[nodiscard]] std::vector<u8> &getData() { return m_data;`

## `libimhex/include/hex/providers/provider.hpp`
Namespaces: `hex::prv`

Types: `class IProviderLoadInterface`, `class IProviderSidebarInterface`, `class IProviderFilePicker`, `class IProviderMenuItems`, `struct MenuEntry`, `class IProviderDataDescription`, `struct Description`, `class Provider`, `class OpenResult`

Function-like declarations:
- `virtual ~IProviderLoadInterface() = default;`
- `virtual bool drawLoadInterface() = 0;`
- `virtual ~IProviderSidebarInterface() = default;`
- `virtual void drawSidebarInterface() = 0;`
- `virtual ~IProviderFilePicker() = default;`
- `virtual bool handleFilePicker() = 0;`
- `std::function<void()> callback;`
- `virtual ~IProviderMenuItems() = default;`
- `virtual std::vector<MenuEntry> getMenuEntries() = 0;`
- `virtual ~IProviderDataDescription() = default;`
- `[[nodiscard]] virtual std::vector<Description> getDataDescription() const = 0;`
- `result.m_result = std::move(errorMessage);`
- `result.m_result = std::move(warningMessage);`
- `Provider();`
- `virtual ~Provider();`
- `Provider(const Provider&) = delete;`
- `Provider& operator=(const Provider&) = delete;`
- `Provider(Provider &&provider) noexcept = default;`
- `Provider& operator=(Provider &&provider) noexcept = default;`
- `* so calling Provider::isAvailable() just after a call to open() that returned true is redundant. * @note This is not related to the EventProviderOpened event * @return true if the provider was opened successfully, else false */ [[nodiscard]] virtual OpenRe…`
- `* it can be opened again later by calling the open() function again. */ virtual void close() = 0;`
- `* @return Generally, if the open() function succeeded and the data source was successfully opened, this * function should return true */ [[nodiscard]] virtual bool isAvailable() const = 0;`
- `[[nodiscard]] virtual bool isReadable() const = 0;`
- `[[nodiscard]] virtual bool isWritable() const = 0;`
- `[[nodiscard]] virtual bool isResizable() const = 0;`
- `* @brief Controls whether the provider can be saved ("saved", not "saved as") * This is mainly used by providers that aren't buffered, and so don't need to be saved * This function will usually return false for providers that aren't writable, but this isn't…`
- `* @brief Controls whether we can dump data from this provider (e.g. "save as", or "export -> .."). * Typically disabled for process with sparse data, like the Process memory provider * where the virtual address space is several TiBs large. * Default impleme…`
- `[[nodiscard]] virtual bool isSavableAsRecent() const { return true;`
- `* @param overlays apply overlays and patches is true. Same as readRaw() if false */ virtual void read(u64 offset, void *buffer, size_t size, bool overlays = true);`
- `virtual void write(u64 offset, const void *buffer, size_t size);`
- `virtual void readRaw(u64 offset, void *buffer, size_t size) = 0;`
- `virtual void writeRaw(u64 offset, const void *buffer, size_t size) = 0;`
- `[[nodiscard]] virtual u64 getActualSize() const = 0;`
- `[[nodiscard]] virtual UnlocalizedString getTypeName() const = 0;`
- `[[nodiscard]] virtual std::string getName() const = 0;`
- `[[nodiscard]] virtual const char* getIcon() const = 0;`
- `bool resize(u64 newSize);`
- `void insert(u64 offset, u64 size);`
- `void remove(u64 offset, u64 size);`
- `virtual void resizeRaw(u64 newSize) { std::ignore = newSize;`
- `virtual void insertRaw(u64 offset, u64 size);`
- `virtual void removeRaw(u64 offset, u64 size);`
- `virtual void save();`
- `virtual void saveAs(const std::fs::path &path);`
- `[[nodiscard]] Overlay *newOverlay();`
- `void deleteOverlay(Overlay *overlay);`
- `void applyOverlays(u64 offset, void *buffer, size_t size) const;`
- `[[nodiscard]] const std::list<std::unique_ptr<Overlay>> &getOverlays() const;`
- `[[nodiscard]] u64 getPageSize() const;`
- `void setPageSize(u64 pageSize);`
- `[[nodiscard]] u32 getPageCount() const;`
- `[[nodiscard]] u32 getCurrentPage() const;`
- `void setCurrentPage(u32 page);`
- `virtual void setBaseAddress(u64 address);`
- `[[nodiscard]] virtual u64 getBaseAddress() const;`
- `[[nodiscard]] virtual u64 getCurrentPageAddress() const;`
- `[[nodiscard]] virtual u64 getSize() const;`
- `[[nodiscard]] virtual std::optional<u32> getPageOfAddress(u64 address) const;`
- `[[nodiscard]] virtual std::variant<std::string, i128> queryInformation(const std::string &category, const std::string &argument);`
- `virtual void undo();`
- `virtual void redo();`
- `[[nodiscard]] virtual bool canUndo() const;`
- `[[nodiscard]] virtual bool canRedo() const;`
- `[[nodiscard]] u32 getID() const;`
- `void setID(u32 id);`
- `[[nodiscard]] virtual nlohmann::json storeSettings(nlohmann::json settings) const;`
- `virtual void loadSettings(const nlohmann::json &settings);`
- `void markDirty(bool dirty = true) { m_dirty = dirty;`
- `[[nodiscard]] bool isDirty() const { return m_dirty;`
- `[[nodiscard]] virtual std::pair<Region, bool> getRegionValidity(u64 address) const;`
- `void skipLoadInterface() { m_skipLoadInterface = true;`
- `[[nodiscard]] bool shouldSkipLoadInterface() const { return m_skipLoadInterface;`
- `[[nodiscard]] virtual undo::Stack& getUndoStack() { return m_undoRedoStack;`

## `libimhex/include/hex/providers/provider_data.hpp`
Namespaces: `hex`, `prv`

Types: `class Provider`, `class PerProvider`

Function-like declarations:
- `PerProvider() { this->onCreate();`
- `PerProvider(const PerProvider&) = delete;`
- `PerProvider(PerProvider&&) = delete;`
- `PerProvider& operator=(const PerProvider&) = delete;`
- `PerProvider& operator=(PerProvider &&) = delete;`
- `~PerProvider() { this->onDestroy();`
- `throw std::invalid_argument("PerProvider::get called with nullptr");`
- `throw std::invalid_argument("PerProvider::set called with nullptr");`
- `EventProviderOpened::subscribe(this, [this](prv::Provider *provider) { auto [it, inserted] = m_data.emplace(provider, T());`
- `EventProviderDeleted::subscribe(this, [this](prv::Provider *provider){ m_onDestroyCallback(provider, m_data.at(provider));`
- `EventImHexClosing::subscribe(this, [this] { m_data.clear();`
- `MovePerProviderData::subscribe(this, [this](prv::Provider *from, prv::Provider *to) { auto node = m_data.extract(from);`
- `EventProviderOpened::unsubscribe(this);`
- `EventProviderDeleted::unsubscribe(this);`
- `EventImHexClosing::unsubscribe(this);`
- `MovePerProviderData::unsubscribe(this);`
- `std::function<void(prv::Provider *, T&)> m_onCreateCallback, m_onDestroyCallback;`

## `libimhex/include/hex/providers/undo_redo/operations/operation.hpp`
Namespaces: `hex::prv`, `hex::prv::undo`

Types: `class Provider`, `class Operation`

Function-like declarations:
- `~Operation() override = default;`
- `virtual void undo(Provider *provider) = 0;`
- `virtual void redo(Provider *provider) = 0;`
- `[[nodiscard]] virtual Region getRegion() const = 0;`
- `[[nodiscard]] virtual std::string format() const = 0;`
- `[[nodiscard]] virtual bool shouldHighlight() const { return true;`

## `libimhex/include/hex/providers/undo_redo/operations/operation_group.hpp`
Namespaces: `hex::prv::undo`

Types: `class OperationGroup`

Function-like declarations:
- `operation->undo(provider);`
- `operation->redo(provider);`
- `auto newRegion = newOperation->getRegion();`
- `u64 m_startAddress = std::numeric_limits<u64>::max();`
- `u64 m_endAddress = std::numeric_limits<u64>::min();`

## `libimhex/include/hex/providers/undo_redo/stack.hpp`
Namespaces: `hex::prv`, `hex::prv::undo`

Types: `class Provider`, `class Stack`

Function-like declarations:
- `explicit Stack(Provider *provider);`
- `void undo(u32 count = 1);`
- `void redo(u32 count = 1);`
- `void groupOperations(u32 count, const UnlocalizedString &unlocalizedName);`
- `void apply(const Stack &otherStack);`
- `void reapply();`
- `[[nodiscard]] bool canUndo() const;`
- `[[nodiscard]] bool canRedo() const;`
- `auto result = this->add(std::make_unique<T>(std::forward<decltype(args)>(args)...));`
- `bool add(std::unique_ptr<Operation> &&operation);`

## `libimhex/include/hex/subcommands/subcommands.hpp`
Namespaces: `hex::subcommands`

Function-like declarations:
- `* (e.g. --help, or when forwarding providers to open to another instance) * and so this function might not return */ void processArguments(const std::vector<std::string> &args);`
- `* @brief Forward the given command to the main instance (might be this instance) * The callback will be executed after EventImHexStartupFinished */ void forwardSubCommand(const std::string &cmdName, const std::vector<std::string> &args);`
- `void registerSubCommand(const std::string &cmdName, const ForwardCommandHandler &handler);`

## `libimhex/include/hex/test/test_provider.hpp`
Namespaces: `hex::test`

Types: `class TestProvider`

Function-like declarations:
- `~TestProvider() override = default;`
- `[[nodiscard]] bool isAvailable() const override { return true;`
- `[[nodiscard]] bool isReadable() const override { return true;`
- `[[nodiscard]] bool isWritable() const override { return false;`
- `[[nodiscard]] bool isResizable() const override { return false;`
- `[[nodiscard]] bool isSavable() const override { return false;`
- `std::memcpy(buffer, m_data->data() + offset, size);`
- `std::memcpy(m_data->data() + offset, buffer, size);`
- `[[nodiscard]] UnlocalizedString getTypeName() const override { return "hex.test.provider.test";`
- `OpenResult open() override { return {};`
- `nlohmann::json storeSettings(nlohmann::json) const override { return {};`
- `void loadSettings(const nlohmann::json &) override {};`

## `libimhex/include/hex/test/tests.hpp`
Namespaces: `hex::test`

Types: `struct Test`, `class Tests`, `class TestSequence`, `struct TestSequenceExecutor`, `class ImGuiTestSequence`, `struct ImGuiTestSequenceExecutor`

Function-like declarations:
- `auto ret = (x);`
- `hex::log::error("Test assert '{}' failed {} at {}:{}", \ fmt::format("" __VA_ARGS__), \ __FILE__, \ __LINE__);`
- `static int addTest(const std::string &name, Function func, bool shouldFail) noexcept;`
- `static std::map<std::string, Test> &get() noexcept;`
- `Tests::addTest(name, func, shouldFail);`
- `TestSequence &operator=(TestSequence &&) = delete;`
- `log::info("Registering ImGui Test");`
- `EventRegisterImGuiTests::subscribe([=](ImGuiTestEngine *engine) { auto test = ImGuiTestEngine_RegisterTest(engine, category.c_str(), name.c_str(), sourceLocation.file_name(), sourceLocation.line());`
- `ImGuiTestSequence &operator=(ImGuiTestSequence &&) = delete;`
- `bool initPluginImpl(std::string name);`

## `libimhex/include/hex/ui/banner.hpp`
Namespaces: `hex`, `impl`

Types: `class BannerBase`, `class Banner`

Function-like declarations:
- `virtual ~BannerBase() = default;`
- `virtual void draw() { drawContent();`
- `virtual void drawContent() = 0;`
- `[[nodiscard]] static std::list<std::unique_ptr<BannerBase>> &getOpenBanners();`
- `void close() { m_shouldClose = true;`
- `[[nodiscard]] bool shouldClose() const { return m_shouldClose;`
- `static std::mutex& getMutex();`
- `std::lock_guard lock(getMutex());`
- `auto toast = std::make_unique<T>(std::forward<Args>(args)...);`
- `getOpenBanners().emplace_back(std::move(toast));`

## `libimhex/include/hex/ui/imgui_imhex_extensions.h`
Namespaces: `ImGuiExt`, `ImGui`

Types: `enum ImGuiCustomCol`, `enum ImGuiCustomStyle`, `class Texture`, `enum class Filter`, `struct ImHexCustomData`, `struct Styles`, `struct ImGuiTestEngine`

Function-like declarations:
- `Texture() = default;`
- `Texture(const Texture&) = delete;`
- `Texture(Texture&& other) noexcept;`
- `[[nodiscard]] static Texture fromImage(const ImU8 *buffer, int size, Filter filter = Filter::Nearest);`
- `[[nodiscard]] static Texture fromImage(std::span<const std::byte> buffer, Filter filter = Filter::Nearest);`
- `[[nodiscard]] static Texture fromImage(const char *path, Filter filter = Filter::Nearest);`
- `[[nodiscard]] static Texture fromImage(const std::fs::path &path, Filter filter = Filter::Nearest);`
- `[[nodiscard]] static Texture fromGLTexture(unsigned int texture, int width, int height);`
- `[[nodiscard]] static Texture fromBitmap(const ImU8 *buffer, int size, int width, int height, Filter filter = Filter::Nearest);`
- `[[nodiscard]] static Texture fromBitmap(std::span<const std::byte> buffer, int width, int height, Filter filter = Filter::Nearest);`
- `[[nodiscard]] static Texture fromSVG(const char *path, int width = 0, int height = 0, Filter filter = Filter::Nearest);`
- `[[nodiscard]] static Texture fromSVG(const std::fs::path &path, int width = 0, int height = 0, Filter filter = Filter::Nearest);`
- `[[nodiscard]] static Texture fromSVG(std::span<const std::byte> buffer, int width = 0, int height = 0, Filter filter = Filter::Nearest);`
- `~Texture();`
- `Texture& operator=(const Texture&) = delete;`
- `Texture& operator=(Texture&& other) noexcept;`
- `[[nodiscard]] std::vector<u8> toBytes() const noexcept;`
- `void reset();`
- `float GetTextWrapPos();`
- `int UpdateStringSizeCallback(ImGuiInputTextCallbackData *data);`
- `bool IconHyperlink(const char *icon, const char *label, const ImVec2 &size_arg = ImVec2(0, 0), ImGuiButtonFlags flags = 0);`
- `bool Hyperlink(const char *label, const ImVec2 &size_arg = ImVec2(0, 0), ImGuiButtonFlags flags = 0);`
- `bool BulletHyperlink(const char *label, const ImVec2 &size_arg = ImVec2(0, 0), ImGuiButtonFlags flags = 0);`
- `bool DescriptionButton(const char *label, const char *description, const char *icon, const ImVec2 &size_arg = ImVec2(0, 0), ImGuiButtonFlags flags = 0);`
- `bool DescriptionButtonProgress(const char *label, const char *description, const char *icon, float fraction, const ImVec2 &size_arg = ImVec2(0, 0), ImGuiButtonFlags flags = 0);`
- `void HelpHover(const char *text, const char *icon = "(?)", ImU32 iconColor = ImGui::GetColorU32(ImGuiCol_ButtonActive));`
- `void UnderlinedText(const char *label, ImColor color = ImGui::GetStyleColorVec4(ImGuiCol_Text), const ImVec2 &size_arg = ImVec2(0, 0));`
- `void UnderwavedText(const char *label, ImColor textColor = ImGui::GetStyleColorVec4(ImGuiCol_Text), ImColor lineColor = ImGui::GetStyleColorVec4(ImGuiCol_Text), const ImVec2 &size_arg = ImVec2(0, 0));`
- `void TextSpinner(const char *label);`
- `void Header(const char *label, bool firstEntry = false);`
- `void HeaderColored(const char *label, ImColor color, bool firstEntry);`
- `bool InfoTooltip(const char *text = "",bool = false);`
- `bool TitleBarButton(const char *label, ImVec2 size_arg);`
- `bool ToolBarButton(const char *symbol, ImVec4 color);`
- `bool IconButton(const char *symbol, ImVec4 color, ImVec2 size_arg = ImVec2(0, 0), ImVec2 iconOffset = ImVec2(0, 0));`
- `bool InputPrefix(const char* label, const char *prefix, std::string &buffer, ImGuiInputTextFlags flags = ImGuiInputTextFlags_None);`
- `bool InputIntegerPrefix(const char* label, const char *prefix, void *value, ImGuiDataType type, const char *format, ImGuiInputTextFlags flags = ImGuiInputTextFlags_None);`
- `bool InputHexadecimal(const char* label, u32 *value, ImGuiInputTextFlags flags = ImGuiInputTextFlags_None);`
- `bool InputHexadecimal(const char* label, u64 *value, ImGuiInputTextFlags flags = ImGuiInputTextFlags_None);`
- `bool SliderBytes(const char *label, u64 *value, u64 min, u64 max, u64 stepSize = 1, ImGuiSliderFlags flags = ImGuiSliderFlags_None);`
- `void OpenPopupInWindow(const char *window_name, const char *popup_name);`
- `void DisableWindowResize(ImGuiDir dir);`
- `ImU32 GetCustomColorU32(ImGuiCustomCol idx, float alpha_mul = 1.0F);`
- `ImVec4 GetCustomColorVec4(ImGuiCustomCol idx, float alpha_mul = 1.0F);`
- `auto &customData = *static_cast<ImHexCustomData *>(ImGui::GetIO().UserData);`
- `float GetCustomStyleFloat(ImGuiCustomStyle idx);`
- `ImVec2 GetCustomStyleVec2(ImGuiCustomStyle idx);`
- `void StyleCustomColorsDark();`
- `void StyleCustomColorsLight();`
- `void StyleCustomColorsClassic();`
- `void ProgressBar(float fraction, ImVec2 size_value = ImVec2(0, 0), float yOffset = 0.0F);`
- `[[nodiscard]] bool IsDarkBackground(const ImColor& bgColor);`
- `ImGui::TextUnformatted(fmt.data(), fmt.data() + fmt.size());`
- `const auto string = fmt::format(fmt::runtime(fmt), std::forward<decltype(args)>(args)...);`
- `ImGui::TextUnformatted(string.c_str(), string.c_str() + string.size());`
- `auto text = fmt::format(fmt::runtime(fmt), std::forward<decltype(args)>(args)...);`
- `ImGui::PushID(text.c_str());`
- `ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2());`
- `ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0F);`
- `ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4());`
- `ImGui::PushItemWidth(ImGui::CalcTextSize(text.c_str()).x + ImGui::GetStyle().FramePadding.x * 2);`
- `ImGui::InputText("##", const_cast<char *>(text.c_str()), text.size() + 1, ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_NoHorizontalScroll);`
- `ImGui::PopItemWidth();`
- `ImGui::PopStyleColor();`
- `ImGui::PopStyleVar(2);`
- `ImGui::PopID();`
- `ImGui::PushStyleColor(ImGuiCol_Text, color.Value);`
- `ImGuiExt::TextFormatted(fmt, std::forward<decltype(args)>(args)...);`
- `ImGui::PushStyleColor(ImGuiCol_Text, IsDarkBackground(backgroundColor) ? 0xFFFFFFFF : 0xFF000000);`
- `ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);`
- `const bool need_backup = ImGuiExt::GetTextWrapPos() < 0.0F;`
- `ImGui::PushTextWrapPos(0.0F);`
- `ImGui::PopTextWrapPos();`
- `auto text = wolv::util::trim(wolv::util::wrapMonospacedString( fmt::format(fmt::runtime(fmt), std::forward<decltype(args)>(args)...), ImGui::CalcTextSize("M").x, std::max(100_scaled, ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ScrollbarSize - ImGui…`
- `auto textSize = ImGui::CalcTextSize(text.c_str());`
- `ImGui::PushItemWidth(textSize.x + ImGui::GetStyle().FramePadding.x * 2);`
- `ImGui::InputTextMultiline( "##", const_cast<char *>(text.c_str()), text.size() + 1, ImVec2(0, textSize.y), ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_NoHorizontalScroll );`
- `void TextUnformattedCentered(const char *text);`
- `TextUnformattedCentered(text.c_str());`
- `auto availableSpace = ImGui::GetContentRegionAvail();`
- `auto textSize = ImGui::CalcTextSize(text.c_str(), nullptr, false, availableSpace.x * 0.75F);`
- `ImGui::SetCursorPosX(((availableSpace - textSize) / 2.0F).x);`
- `ImGui::PushTextWrapPos(availableSpace.x * 0.75F);`
- `ImGuiExt::TextFormattedWrapped("{}", text);`
- `bool InputTextIcon(const char* label, const char *icon, std::string &buffer, ImGuiInputTextFlags flags = ImGuiInputTextFlags_None);`
- `bool InputTextIconHint(const char* label, const char *icon, const char *hint, std::string &buffer, ImGuiInputTextFlags flags = ImGuiInputTextFlags_None);`
- `bool InputScalarCallback(const char* label, ImGuiDataType data_type, void* p_data, const char* format, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data);`
- `void HideTooltip();`
- `bool BitCheckbox(const char* label, bool* v);`
- `bool DimmedButton(const char* label, ImVec2 size = ImVec2(0, 0), ImGuiButtonFlags flags = ImGuiButtonFlags_None);`
- `bool DimmedIconButton(const char *symbol, ImVec4 color, ImVec2 size = ImVec2(0, 0), ImVec2 iconOffset = ImVec2(0, 0));`
- `bool DimmedButtonToggle(const char *icon, bool *v, ImVec2 size = ImVec2(0, 0), ImVec2 iconOffset = ImVec2(0, 0));`
- `bool DimmedIconToggle(const char *icon, bool *v);`
- `bool DimmedIconToggle(const char *iconOn, const char *iconOff, bool *v);`
- `void TextOverlay(const char *text, ImVec2 pos, float maxWidth = -1);`
- `bool BeginBox();`
- `void EndBox();`
- `bool BeginSubWindow(const char *label, bool *collapsed = nullptr, ImVec2 size = ImVec2(0, 0), ImGuiChildFlags flags = ImGuiChildFlags_None);`
- `void EndSubWindow();`
- `auto width = ImGui::GetWindowWidth();`
- `ImGui::SetCursorPosX(width / 9);`
- `leftButtonCallback();`
- `ImGui::SameLine();`
- `ImGui::SetCursorPosX(width / 9 * 5);`
- `rightButtonCallback();`
- `bool VSliderAngle(const char* label, ImVec2& size, float* v_rad, float v_degrees_min, float v_degrees_max, const char* format, ImGuiSliderFlags flags);`
- `bool InputFilePicker(const char *label, std::fs::path &path, const std::vector<hex::fs::ItemFilter> &validExtensions);`
- `bool ToggleSwitch(const char *label, bool *v);`
- `bool ToggleSwitch(const char *label, bool v);`
- `bool PopupTitleBarButton(const char* label, bool p_enabled);`
- `void PopupTitleBarText(const char* text);`
- `else if constexpr (std::same_as<T, u16>) return ImGuiDataType_U16;`
- `else if constexpr (std::same_as<T, u32>) return ImGuiDataType_U32;`
- `else if constexpr (std::same_as<T, u64>) return ImGuiDataType_U64;`
- `else if constexpr (std::same_as<T, i8>) return ImGuiDataType_S8;`
- `else if constexpr (std::same_as<T, i16>) return ImGuiDataType_S16;`
- `else if constexpr (std::same_as<T, i32>) return ImGuiDataType_S32;`
- `else if constexpr (std::same_as<T, i64>) return ImGuiDataType_S64;`
- `else if constexpr (std::same_as<T, float>) return ImGuiDataType_Float;`
- `else if constexpr (std::same_as<T, double>) return ImGuiDataType_Double;`
- `else static_assert(hex::always_false<T>::value, "Invalid data type!");`
- `else if constexpr (std::same_as<T, u16>) return "h";`
- `else if constexpr (std::same_as<T, u32>) return "l";`
- `else if constexpr (std::same_as<T, u64>) return "ll";`
- `else if constexpr (std::same_as<T, i8>) return "hh";`
- `else if constexpr (std::same_as<T, i16>) return "h";`
- `else if constexpr (std::same_as<T, i32>) return "l";`
- `else if constexpr (std::same_as<T, i64>) return "ll";`
- `ImGuiTestEngine() = delete;`
- `static void setEnabled(bool enabled);`
- `[[nodiscard]] static bool isEnabled();`
- `bool InputText(const char* label, std::string &buffer, ImGuiInputTextFlags flags = ImGuiInputTextFlags_None);`
- `bool InputText(const char *label, std::u8string &buffer, ImGuiInputTextFlags flags = ImGuiInputTextFlags_None);`
- `bool InputTextMultiline(const char* label, std::string &buffer, const ImVec2& size = ImVec2(0, 0), ImGuiInputTextFlags flags = ImGuiInputTextFlags_None);`
- `bool InputTextWithHint(const char *label, const char *hint, std::string &buffer, ImGuiInputTextFlags flags = ImGuiInputTextFlags_None);`

## `libimhex/include/hex/ui/popup.hpp`
Namespaces: `hex`, `impl`

Types: `class PopupBase`, `class Popup`

Function-like declarations:
- `virtual ~PopupBase() = default;`
- `virtual void drawContent() = 0;`
- `[[nodiscard]] virtual ImGuiWindowFlags getFlags() const { return ImGuiWindowFlags_None;`
- `[[nodiscard]] static std::vector<std::unique_ptr<PopupBase>> &getOpenPopups();`
- `static std::mutex& getMutex();`
- `std::lock_guard lock(getMutex());`
- `auto popup = std::make_unique<T>(std::forward<Args>(args)...);`
- `getOpenPopups().emplace_back(std::move(popup));`

## `libimhex/include/hex/ui/toast.hpp`
Namespaces: `hex`, `impl`

Types: `class ToastBase`, `class Toast`

Function-like declarations:
- `virtual ~ToastBase() = default;`
- `virtual void draw() { drawContent();`
- `virtual void drawContent() = 0;`
- `[[nodiscard]] static std::list<std::unique_ptr<ToastBase>> &getQueuedToasts();`
- `static std::mutex& getMutex();`
- `TaskManager::doLater([=] { auto toast = std::make_unique<T>(args...);`

## `libimhex/include/hex/ui/view.hpp`
Namespaces: `hex`

Types: `class View`, `class Window`, `class Special`, `class Floating`, `class Scrolling`, `class Modal`, `class FullScreen`

Function-like declarations:
- `explicit View(UnlocalizedString unlocalizedName, const char *icon);`
- `virtual ~View() = default;`
- `* @note Do not override this method. Override drawContent() instead */ virtual void draw(ImGuiWindowFlags extraFlags = ImGuiWindowFlags_None) = 0;`
- `virtual void drawContent() = 0;`
- `[[nodiscard]] virtual bool shouldDraw() const;`
- `* drawn in the drawAlwaysVisibleContent() function. * @return True if the view should be processed, false otherwise */ [[nodiscard]] virtual bool shouldProcess() const;`
- `[[nodiscard]] virtual bool hasViewMenuItemEntry() const;`
- `[[nodiscard]] virtual ImVec2 getMinSize() const;`
- `[[nodiscard]] virtual ImVec2 getMaxSize() const;`
- `[[nodiscard]] virtual ImGuiWindowFlags getWindowFlags() const;`
- `[[nodiscard]] virtual View* getMenuItemInheritView() const { return nullptr;`
- `[[nodiscard]] const char *getIcon() const { return m_icon;`
- `[[nodiscard]] const UnlocalizedString& getUnlocalizedName() const;`
- `[[nodiscard]] std::string getName() const;`
- `[[nodiscard]] virtual bool shouldDefaultFocus() const { return false;`
- `[[nodiscard]] virtual bool shouldStoreWindowState() const { return true;`
- `[[nodiscard]] bool &getWindowOpenState();`
- `[[nodiscard]] const bool &getWindowOpenState() const;`
- `[[nodiscard]] bool isFocused() const { return m_focused;`
- `[[nodiscard]] static std::string toWindowName(const UnlocalizedString &unlocalizedName);`
- `[[nodiscard]] static const View* getLastFocusedView();`
- `static void discardNavigationRequests();`
- `void bringToFront();`
- `[[nodiscard]] bool didWindowJustOpen();`
- `void setWindowJustOpened(bool state);`
- `[[nodiscard]] bool didWindowJustClose();`
- `void setWindowJustClosed(bool state);`
- `void trackViewState();`
- `void setFocused(bool focused);`
- `virtual void drawHelpText() = 0;`
- `void draw(ImGuiWindowFlags extraFlags = ImGuiWindowFlags_None) override;`
- `void draw(ImGuiWindowFlags extraFlags = ImGuiWindowFlags_None) final;`
- `[[nodiscard]] bool shouldStoreWindowState() const override { return false;`
- `[[nodiscard]] virtual bool hasCloseButton() const { return true;`

## `libimhex/include/hex/ui/widgets.hpp`
Namespaces: `hex::ui`

Types: `class SearchableWidget`

Function-like declarations:
- `std::function<bool(const std::string&, const T&)> m_comparator;`
