set(QomposeCommon_RESOURCES

	data.qrc
	dictionaries.qrc
	icons.qrc

)

set(QomposeCommon_SOURCES

	Application.cpp
	Application.h
	Defines.h
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
	editor/DecoratedTextEdit.cpp
	editor/DecoratedTextEdit.h
	editor/Editor.cpp
	editor/Editor.h
	editor/Gutter.cpp
	editor/Gutter.h

	editor/spell/SpellChecker.cpp
	editor/spell/SpellChecker.h
	editor/spell/SpellCheckerWorker.cpp
	editor/spell/SpellCheckerWorker.h
	editor/spell/Utils.cpp
	editor/spell/Utils.h

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
	gui/NotificationLabel.cpp
	gui/NotificationLabel.h
	gui/StatusBar.cpp
	gui/StatusBar.h

	gui/dock/BrowserDockWidget.cpp
	gui/dock/BrowserDockWidget.h
	gui/dock/BrowserModel.cpp
	gui/dock/BrowserModel.h
	gui/dock/BrowserView.cpp
	gui/dock/BrowserView.h

	gui/menus/MainMenu.cpp
	gui/menus/MainMenu.h
	gui/menus/RecentMenu.cpp
	gui/menus/RecentMenu.h

	syntax/Lexer.cpp
	syntax/Lexer.h
	syntax/SyntaxHighlighter.cpp
	syntax/SyntaxHighlighter.h

	util/DocumentWriter.cpp
	util/DocumentWriter.h
	util/FindQuery.cpp
	util/FindQuery.h
	util/FontMetrics.cpp
	util/FontMetrics.h
	util/Hotkey.cpp
	util/Hotkey.h
	util/HotkeyMap.cpp
	util/HotkeyMap.h
	util/ReplaceQuery.cpp
	util/ReplaceQuery.h
	util/Settings.cpp
	util/Settings.h

)

qt5_add_resources(QomposeCommon_RESOURCES_RCC ${QomposeCommon_RESOURCES})

add_library(QomposeCommon STATIC

	${QomposeCommon_RESOURCES_RCC}
	${QomposeCommon_SOURCES}

)

qt5_use_modules(QomposeCommon Core Gui Widgets Network PrintSupport)
