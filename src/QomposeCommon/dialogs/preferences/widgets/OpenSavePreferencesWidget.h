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

#ifndef INCLUDE_QOMPOSECOMMON_DIALOGS_PREFERENCES_WIDGETS_OPEN_SAVE_PREFERENCES_WIDGET_H
#define INCLUDE_QOMPOSECOMMON_DIALOGS_PREFERENCES_WIDGETS_OPEN_SAVE_PREFERENCES_WIDGET_H

#include "QomposeCommon/dialogs/preferences/widgets/PreferencesWidget.h"

class QGridLayout;
class QGroupBox;
class QCheckBox;

namespace qompose
{
class Settings;

/*!
 * \brief This widget provides a UI for configuring for open/save preferences.
 */
class OpenSavePreferencesWidget : public PreferencesWidget
{
public:
	/*!
	 * This is our default constructor, which creates a new instance of our
	 * open/save preferences widget.
	 *
	 * \param s The settings instance to use to save preferences values.
	 * \param p The parent widget to use for this widget.
	 * \param f The window flags to use for this widget.
	 */
	OpenSavePreferencesWidget(Settings *s, QWidget *p = nullptr,
	                          Qt::WindowFlags f = nullptr);

	OpenSavePreferencesWidget(const OpenSavePreferencesWidget &) = delete;

	virtual ~OpenSavePreferencesWidget() = default;

	OpenSavePreferencesWidget &
	operator=(const OpenSavePreferencesWidget &) = delete;

	/*!
	 * We implement our superclass's apply() function to save our various
	 * open/save preferences values using our settings instance.
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
	QCheckBox *stripTrailingSpacesCheckBox;

	/*!
	 * This function initializes our widget's GUI by creating the various
	 * widgets we contain, and adding them to our layout.
	 */
	void initializeGUI();
};
}

#endif
