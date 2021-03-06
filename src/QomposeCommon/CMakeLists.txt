set(QomposeCommon_RESOURCES

	data.qrc
	dictionaries.qrc
	icons.qrc

)

set(QomposeCommon_SOURCES

	Application.cpp
	Application.h
	Defines.h
	Types.cpp
	Types.h
	Window.cpp
	Window.h

	dialogs/AboutDialog.cpp
	dialogs/AboutDialog.h
	dialogs/EncodingDialog.cpp
	dialogs/EncodingDialog.h
	dialogs/FileDialog.cpp
	dialogs/FileDialog.h
	dialogs/FindDialog.cpp
	dialogs/FindDialog.h
	dialogs/GoToDialog.cpp
	dialogs/GoToDialog.h
	dialogs/ReplaceDialog.cpp
	dialogs/ReplaceDialog.h

	dialogs/preferences/PreferencesDialog.cpp
	dialogs/preferences/PreferencesDialog.h
	dialogs/preferences/PreferencesListModel.cpp
	dialogs/preferences/PreferencesListModel.h
	dialogs/preferences/PreferencesListView.cpp
	dialogs/preferences/PreferencesListView.h

	dialogs/preferences/widgets/EditorPreferencesWidget.cpp
	dialogs/preferences/widgets/EditorPreferencesWidget.h
	dialogs/preferences/widgets/GeneralPreferencesWidget.cpp
	dialogs/preferences/widgets/GeneralPreferencesWidget.h
	dialogs/preferences/widgets/OpenSavePreferencesWidget.cpp
	dialogs/preferences/widgets/OpenSavePreferencesWidget.h
	dialogs/preferences/widgets/PreferencesScrollArea.cpp
	dialogs/preferences/widgets/PreferencesScrollArea.h
	dialogs/preferences/widgets/PreferencesWidget.cpp
	dialogs/preferences/widgets/PreferencesWidget.h

	editor/Buffer.cpp
	editor/Buffer.h
	editor/Editor.cpp
	editor/Editor.h
	editor/Gutter.cpp
	editor/Gutter.h

	editor/algorithm/General.cpp
	editor/algorithm/General.h
	editor/algorithm/Indentation.cpp
	editor/algorithm/Indentation.h
	editor/algorithm/Movement.cpp
	editor/algorithm/Movement.h

	editor/pane/Pane.cpp
	editor/pane/Pane.h
	editor/pane/PaneModel.cpp
	editor/pane/PaneModel.h
	editor/pane/PaneView.cpp
	editor/pane/PaneView.h
	editor/pane/TabbedPaneView.cpp
	editor/pane/TabbedPaneView.h

	editor/search/applyAlgorithm.h
	editor/search/Find.cpp
	editor/search/Find.h
	editor/search/Query.cpp
	editor/search/Query.h
	editor/search/Replace.cpp
	editor/search/Replace.h

	editor/spell/SpellChecker.cpp
	editor/spell/SpellChecker.h
	editor/spell/SpellCheckerWorker.cpp
	editor/spell/SpellCheckerWorker.h

	fs/DocumentWriter.cpp
	fs/DocumentWriter.h

	gui/BufferWidget.cpp
	gui/BufferWidget.h
	gui/ColorPickerButton.cpp
	gui/ColorPickerButton.h
	gui/EllipsizedLabel.cpp
	gui/EllipsizedLabel.h
	gui/FontPickerButton.cpp
	gui/FontPickerButton.h
	gui/GUIUtils.cpp
	gui/GUIUtils.h
	gui/StatusBar.cpp
	gui/StatusBar.h

	gui/dock/BrowserDockWidget.cpp
	gui/dock/BrowserDockWidget.h
	gui/dock/BrowserModel.cpp
	gui/dock/BrowserModel.h
	gui/dock/BrowserView.cpp
	gui/dock/BrowserView.h

	gui/menus/EncodingMenu.cpp
	gui/menus/EncodingMenu.h
	gui/menus/MainMenu.cpp
	gui/menus/MainMenu.h
	gui/menus/MenuDescriptors.cpp
	gui/menus/MenuDescriptors.h
	gui/menus/RecentMenu.cpp
	gui/menus/RecentMenu.h

	hotkey/Hotkey.cpp
	hotkey/Hotkey.h
	hotkey/HotkeyedWidget.h
	hotkey/HotkeyMap.cpp
	hotkey/HotkeyMap.h

	syntax/Lexer.cpp
	syntax/Lexer.h
	syntax/SyntaxHighlighter.cpp
	syntax/SyntaxHighlighter.h

	thread/IOPriorityDefines.h
	thread/LowPriorityThread.cpp
	thread/LowPriorityThread.h
	thread/Utils.cpp
	thread/Utils.h

	util/ConfigurationWatcher.cpp
	util/ConfigurationWatcher.hpp
	util/Encoding.cpp
	util/Encoding.h
	util/FontMetrics.cpp
	util/FontMetrics.h

)

qt5_add_resources(QomposeCommon_RESOURCES_RCC ${QomposeCommon_RESOURCES})

add_library(QomposeCommon STATIC

	${QomposeCommon_RESOURCES_RCC}
	${QomposeCommon_SOURCES}

)
target_link_libraries(QomposeCommon qompose-core ${qompose_LIBRARIES})
qt5_use_modules(QomposeCommon Core Gui Widgets Network PrintSupport)
