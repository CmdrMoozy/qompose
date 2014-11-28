set(QomposeCommon_RESOURCES

	data.qrc
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
	editor/QomposeDecoratedTextEdit.cpp
	editor/QomposeDecoratedTextEdit.h
	editor/QomposeEditor.cpp
	editor/QomposeEditor.h
	editor/QomposeGutter.cpp
	editor/QomposeGutter.h

	gui/BufferWidget.cpp
	gui/BufferWidget.h
	gui/QomposeColorPickerButton.cpp
	gui/QomposeColorPickerButton.h
	gui/QomposeFontPickerButton.cpp
	gui/QomposeFontPickerButton.h
	gui/QomposeGUIUtils.cpp
	gui/QomposeGUIUtils.h
	gui/QomposeNotificationLabel.cpp
	gui/QomposeNotificationLabel.h
	gui/QomposeStatusBar.cpp
	gui/QomposeStatusBar.h

	gui/menus/MainMenu.cpp
	gui/menus/MainMenu.h
	gui/menus/QomposeRecentMenu.cpp
	gui/menus/QomposeRecentMenu.h

	syntax/QomposeLexer.cpp
	syntax/QomposeLexer.h
	syntax/QomposeSyntaxHighlighter.cpp
	syntax/QomposeSyntaxHighlighter.h

	util/QomposeDocumentWriter.cpp
	util/QomposeDocumentWriter.h
	util/QomposeFindQuery.cpp
	util/QomposeFindQuery.h
	util/QomposeFontMetrics.cpp
	util/QomposeFontMetrics.h
	util/QomposeHotkey.cpp
	util/QomposeHotkey.h
	util/QomposeHotkeyMap.h
	util/QomposeReplaceQuery.cpp
	util/QomposeReplaceQuery.h
	util/QomposeSettings.cpp
	util/QomposeSettings.h

)

qt5_add_resources(QomposeCommon_RESOURCES_RCC ${QomposeCommon_RESOURCES})

add_library(QomposeCommon STATIC

	${QomposeCommon_RESOURCES_RCC}
	${QomposeCommon_SOURCES}

)

qt5_use_modules(QomposeCommon Core Gui Widgets Network PrintSupport)
