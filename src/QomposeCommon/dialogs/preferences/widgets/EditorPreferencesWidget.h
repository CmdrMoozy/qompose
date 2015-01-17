/*
 * Qompose - A simple programmer's text editor.
 * Copyright (C) 2013 Axel Rasmussen
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef INCLUDE_QOMPOSECOMMON_DIALOGS_PREFERENCES_WIDGETS_EDITOR_PREFERENCES_WIDGET_H
#define INCLUDE_QOMPOSECOMMON_DIALOGS_PREFERENCES_WIDGETS_EDITOR_PREFERENCES_WIDGET_H

#include "QomposeCommon/dialogs/preferences/widgets/PreferencesWidget.h"

class QGroupBox;
class QGridLayout;
class QCheckBox;
class QLabel;
class QSpinBox;
class QButtonGroup;
class QRadioButton;

namespace qompose
{
class ColorPickerButton;
class FontPickerButton;
class Settings;

/*!
 * \brief This class implements a widget to configure editor-related settings.
 */
class EditorPreferencesWidget : public PreferencesWidget
{
public:
	EditorPreferencesWidget(Settings *s, QWidget *p = nullptr,
	                        Qt::WindowFlags f = nullptr);
	virtual ~EditorPreferencesWidget();

	virtual void apply();
	virtual void discardChanges();

private:
	QGridLayout *layout;

	QGroupBox *generalGroupBox;
	QGridLayout *generalLayout;
	QCheckBox *showGutterCheckBox;
	QLabel *editorFontLabel;
	FontPickerButton *editorFontButton;
	QLabel *indentationWidthLabel;
	QSpinBox *indentationWidthSpinBox;
	QLabel *indentationModeLabel;
	QButtonGroup *indentationModeButtonGroup;
	QRadioButton *indentationModeTabsRadioButton;
	QRadioButton *indentationModeSpacesRadioButton;

	QGroupBox *lineWrapGuideGroupBox;
	QGridLayout *lineWrapGuideLayout;
	QCheckBox *lineWrapGuideCheckBox;
	QLabel *lineWrapGuideWidthLabel;
	QSpinBox *lineWrapGuideWidthSpinBox;
	QLabel *lineWrapGuideColorLabel;
	ColorPickerButton *lineWrapGuideColorButton;

	QGroupBox *colorsGroupBox;
	QGridLayout *colorsLayout;
	QLabel *editorFGLabel;
	ColorPickerButton *editorFGButton;
	QLabel *editorBGLabel;
	ColorPickerButton *editorBGButton;
	QLabel *currentLineBGLabel;
	ColorPickerButton *currentLineBGButton;
	QLabel *gutterFGLabel;
	ColorPickerButton *gutterFGButton;
	QLabel *gutterBGLabel;
	ColorPickerButton *gutterBGButton;

	EditorPreferencesWidget(const EditorPreferencesWidget &);
	EditorPreferencesWidget &operator=(const EditorPreferencesWidget &);

	void initializeGUI();

	QString getSelectedIndentationMode() const;
	void setSelectedIndentaionMode(const QString &);
};
}

#endif
