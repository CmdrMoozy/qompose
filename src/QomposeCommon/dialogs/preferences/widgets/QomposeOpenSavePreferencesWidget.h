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

#ifndef INCLUDE_QOMPOSE_OPEN_SAVE_PREFERENCES_WIDGET_H
#define INCLUDE_QOMPOSE_OPEN_SAVE_PREFERENCES_WIDGET_H

#include "QomposeCommon/dialogs/preferences/widgets/QomposePreferencesWidget.h"

class QGridLayout;
class QGroupBox;
class QCheckBox;

class QomposeSettings;

/*!
 * \brief This widget provides a UI for configuring for open/save preferences.
 */
class QomposeOpenSavePreferencesWidget : public QomposePreferencesWidget
{
	public:
		QomposeOpenSavePreferencesWidget(QomposeSettings *s,
			QWidget *p = nullptr, Qt::WindowFlags f = nullptr);
		virtual ~QomposeOpenSavePreferencesWidget();

		virtual void apply();
		virtual void discardChanges();

	private:
		QGridLayout *layout;

		QGroupBox *generalGroupBox;
		QGridLayout *generalLayout;
		QCheckBox *stripTrailingSpacesCheckBox;

		QomposeOpenSavePreferencesWidget(
			const QomposeOpenSavePreferencesWidget &);
		QomposeOpenSavePreferencesWidget &operator=(
			const QomposeOpenSavePreferencesWidget &);

		void initializeGUI();
};

#endif
