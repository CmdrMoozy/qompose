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
/*!
 * \brief This class implements a widget to configure editor-related settings.
 */
class EditorPreferencesWidget : public PreferencesWidget
{
public:
	/*!
	 * This is our default constructor, which creates a new instance of our
	 * editor preferences widget.
	 *
	 * \param p The parent widget to use for this widget.
	 * \param f The window flags to use for this widget.
	 */
	EditorPreferencesWidget(QWidget *p = nullptr,
	                        Qt::WindowFlags f = nullptr);

	EditorPreferencesWidget(const EditorPreferencesWidget &) = delete;

	virtual ~EditorPreferencesWidget() = default;

	EditorPreferencesWidget &
	operator=(const EditorPreferencesWidget &) = delete;

	/*!
	 * We implement our superclass's apply() function to save our various
	 * editor preferences values using our settings instance.
	 */
	virtual void apply();

	/*!
	 * We implement our superclass's discardChanges() function to discard
	 * any preferences changes that may have been made to our widget by
	 * reloading the existing values from our settings instance.
	 */
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

	/*!
	 * This function initializes our widget's GUI by creating the various
	 * widgets we contain, and adding them to our layout.
	 */
	void initializeGUI();

	/*!
	 * This is a small utility function which returns the currently
	 * selected indentation mode as a string.
	 *
	 * \return The currently selected indentation mode as a string.
	 */
	QString getSelectedIndentationMode() const;

	/*!
	 * This is a small utility function which sets the currently selected
	 * indentation mode in the UI based upon the given string value.
	 *
	 * \param mode The new indentation mode.
	 */
	void setSelectedIndentaionMode(const QString &mode);
};
}

#endif
