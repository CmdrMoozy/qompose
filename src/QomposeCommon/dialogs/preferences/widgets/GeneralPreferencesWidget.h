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

#ifndef INCLUDE_QOMPOSECOMMON_DIALOGS_PREFERENCES_WIDGETS_GENERAL_PREFERENCES_WIDGET_H
#define INCLUDE_QOMPOSECOMMON_DIALOGS_PREFERENCES_WIDGETS_GENERAL_PREFERENCES_WIDGET_H

#include "QomposeCommon/dialogs/preferences/widgets/PreferencesWidget.h"

class QGridLayout;
class QCheckBox;
class QLabel;
class QSpinBox;

namespace qompose
{

class Settings;

/*!
 * \brief This class implements a widget to configure general options.
 */
class GeneralPreferencesWidget : public PreferencesWidget
{
public:
	GeneralPreferencesWidget(Settings *s, QWidget *p = nullptr,
		Qt::WindowFlags f = nullptr);
	virtual ~GeneralPreferencesWidget();

	virtual void apply();
	virtual void discardChanges();

private:
	QGridLayout *layout;
	QCheckBox *statusBarCheckBox;
	QLabel *recentListSizeLabel;
	QSpinBox *recentListSizeSpinBox;
	QCheckBox *saveWindowAttribsCheckBox;

	GeneralPreferencesWidget(const GeneralPreferencesWidget &);
	GeneralPreferencesWidget &operator=(const GeneralPreferencesWidget &);

	void initializeGUI();
};

}

#endif
