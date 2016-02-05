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

#ifndef INCLUDE_QOMPOSECOMMON_DIALOGS_PREFERENCES_WIDGETS_PREFERENCES_WIDGET_H
#define INCLUDE_QOMPOSECOMMON_DIALOGS_PREFERENCES_WIDGETS_PREFERENCES_WIDGET_H

#include <QWidget>
#include <QIcon>
#include <QString>

namespace qompose
{
/*!
 * \brief This class provides a common interface for all preferences widgets.
 */
class PreferencesWidget : public QWidget
{
public:
	/*!
	 * This is our default constructor, which creates a new, empty
	 * preferences widget.
	 *
	 * \param p This widget's parent widget.
	 * \param f The window flags to use for this widget.
	 */
	PreferencesWidget(QWidget *p = nullptr, Qt::WindowFlags f = nullptr);

	PreferencesWidget(const PreferencesWidget &) = delete;

	virtual ~PreferencesWidget();

	PreferencesWidget &operator=(const PreferencesWidget &) = delete;

	/*!
	 * This function returns our widget's current display icon, for use in
	 * e.g. a preferences list view.
	 *
	 * \return Our widget's icon.
	 */
	QIcon getPreferencesIcon() const;

	/*!
	 * This function sets the icon our widget should display e.g. when in a
	 * preferences list view.
	 *
	 * \param i The new icon for our widget to use.
	 */
	void setPreferencesIcon(const QIcon &i);

	/*!
	 * This function returns the title identifying this preferences widget.
	 * This title can be displayed e.g. in a dialog title, or in a
	 * preferences list view.
	 *
	 * \return Our widget's title.
	 */
	QString getPreferencesTitle() const;

	/*!
	 * This function sets the title used to identify this widget. This
	 * title can be displayed e.g. in a dialog title, or in a preferences
	 * list view.
	 *
	 * \param t The new title for our widget to use.
	 */
	void setPreferencesTitle(const QString &t);

	/*!
	 * This function should be implemented by subclasses, and
	 * should apply all of that widget's settings to the
	 * QomposeSettings instance given in the constructor.
	 */
	virtual void apply() = 0;

	/*!
	 * This function should be implemented by subclasses, and
	 * should discard any settings values in the widget, replacing
	 * them with the existing values stored in the QomposeSettings
	 * instance given in the constructor.
	 */
	virtual void discardChanges() = 0;

private:
	QIcon icon;
	QString title;
};
}

#endif
