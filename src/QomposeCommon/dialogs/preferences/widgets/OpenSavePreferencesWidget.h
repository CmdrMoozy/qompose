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

class QomposeSettings;

namespace qompose
{

/*!
 * \brief This widget provides a UI for configuring for open/save preferences.
 */
class OpenSavePreferencesWidget : public PreferencesWidget
{
public:
	OpenSavePreferencesWidget(QomposeSettings *s, QWidget *p = nullptr,
		Qt::WindowFlags f = nullptr);
	virtual ~OpenSavePreferencesWidget();

	virtual void apply();
	virtual void discardChanges();

private:
	QGridLayout *layout;

	QGroupBox *generalGroupBox;
	QGridLayout *generalLayout;
	QCheckBox *stripTrailingSpacesCheckBox;

	OpenSavePreferencesWidget(const OpenSavePreferencesWidget &);
	OpenSavePreferencesWidget &operator=(
		const OpenSavePreferencesWidget &);

	void initializeGUI();
};

}

#endif
