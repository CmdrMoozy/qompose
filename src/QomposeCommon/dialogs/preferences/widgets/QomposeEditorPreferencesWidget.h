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

#ifndef INCLUDE_QOMPOSE_EDITOR_PREFERENCES_WIDGET_H
#define INCLUDE_QOMPOSE_EDITOR_PREFERENCES_WIDGET_H

#include "QomposeCommon/dialogs/preferences/widgets/QomposePreferencesWidget.h"

class QGroupBox;
class QGridLayout;
class QCheckBox;
class QLabel;
class QSpinBox;

class QomposeSettings;
class QomposeColorPickerButton;
class QomposeFontPickerButton;

/*!
 * \brief This class implements a widget to configure editor-related settings.
 */
class QomposeEditorPreferencesWidget : public QomposePreferencesWidget
{
	public:
		QomposeEditorPreferencesWidget(QomposeSettings *s,
			QWidget *p = nullptr, Qt::WindowFlags f = nullptr);
		virtual ~QomposeEditorPreferencesWidget();

		virtual void apply();
		virtual void discardChanges();

	private:
		QGridLayout *layout;

		QGroupBox *generalGroupBox;
		QGridLayout *generalLayout;
		QCheckBox *showGutterCheckBox;
		QLabel *editorFontLabel;
		QomposeFontPickerButton *editorFontButton;
		QLabel *tabWidthLabel;
		QSpinBox *tabWidthSpinBox;

		QGroupBox *lineWrapGuideGroupBox;
		QGridLayout *lineWrapGuideLayout;
		QCheckBox *lineWrapGuideCheckBox;
		QLabel *lineWrapGuideWidthLabel;
		QSpinBox *lineWrapGuideWidthSpinBox;
		QLabel *lineWrapGuideColorLabel;
		QomposeColorPickerButton *lineWrapGuideColorButton;

		QGroupBox *colorsGroupBox;
		QGridLayout *colorsLayout;
		QLabel *editorFGLabel;
		QomposeColorPickerButton *editorFGButton;
		QLabel *editorBGLabel;
		QomposeColorPickerButton *editorBGButton;
		QLabel *currentLineBGLabel;
		QomposeColorPickerButton *currentLineBGButton;
		QLabel *gutterFGLabel;
		QomposeColorPickerButton *gutterFGButton;
		QLabel *gutterBGLabel;
		QomposeColorPickerButton *gutterBGButton;

		QomposeEditorPreferencesWidget(
			const QomposeEditorPreferencesWidget &);
		QomposeEditorPreferencesWidget &operator=(
			const QomposeEditorPreferencesWidget &);

		void initializeGUI();
};

#endif
