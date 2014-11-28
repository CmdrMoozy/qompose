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

	dialogs/preferences/QomposePreferencesDialog.cpp
	dialogs/preferences/QomposePreferencesDialog.h
	dialogs/preferences/QomposePreferencesListModel.cpp
	dialogs/preferences/QomposePreferencesListModel.h
	dialogs/preferences/QomposePreferencesListView.cpp
	dialogs/preferences/QomposePreferencesListView.h

	dialogs/preferences/widgets/QomposeEditorPreferencesWidget.cpp
	dialogs/preferences/widgets/QomposeEditorPreferencesWidget.h
	dialogs/preferences/widgets/QomposeGeneralPreferencesWidget.cpp
	dialogs/preferences/widgets/QomposeGeneralPreferencesWidget.h
	dialogs/preferences/widgets/QomposeOpenSavePreferencesWidget.cpp
	dialogs/preferences/widgets/QomposeOpenSavePreferencesWidget.h
	dialogs/preferences/widgets/QomposePreferencesScrollArea.cpp
	dialogs/preferences/widgets/QomposePreferencesScrollArea.h
	dialogs/preferences/widgets/QomposePreferencesWidget.cpp
	dialogs/preferences/widgets/QomposePreferencesWidget.h

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
