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

#include "QomposePreferencesWidget.h"

#include "QomposeCommon/util/QomposeSettings.h"

/*!
 * This is our default constructor, which creates a new, empty preferences
 * widget.
 *
 * \param s The settings instance to use to persist our settings.
 * \param p This widget's parent widget.
 * \param f The window flags to use for this widget.
 */
QomposePreferencesWidget::QomposePreferencesWidget(QomposeSettings *s,
	QWidget *p, Qt::WindowFlags f)
	: QWidget(p, f), settings(s), icon(QIcon()), title(QString())
{
}

/*!
 * This is our default destructor, which cleans up & destroys our widget.
 */
QomposePreferencesWidget::~QomposePreferencesWidget()
{
}

/*!
 * This function returns our widget's current display icon, for use in e.g.
 * a preferences list view.
 *
 * \return Our widget's icon.
 */
QIcon QomposePreferencesWidget::getPreferencesIcon() const
{
	return icon;
}

/*!
 * This function sets the icon our widget should display e.g. when in a
 * preferences list view.
 *
 * \param i The new icon for our widget to use.
 */
void QomposePreferencesWidget::setPreferencesIcon(const QIcon &i)
{
	icon = i;
}

/*!
 * This function returns the title identifying this preferences widget.
 * This title can be displayed e.g. in a dialog title, or in a preferences
 * list view.
 *
 * \return Our widget's title.
 */
QString QomposePreferencesWidget::getPreferencesTitle() const
{
	return title;
}

/*!
 * This function sets the title used to identify this widget. This title can
 * be displayed e.g. in a dialog title, or in a preferences list view.
 *
 * \param t The new title for our widget to use.
 */
void QomposePreferencesWidget::setPreferencesTitle(const QString &t)
{
	title = t;
}

/*!
 * This function returns the settings instance our widget is using to
 * persist settings.
 *
 * \return Our widget's settings instance.
 */
QomposeSettings *QomposePreferencesWidget::getSettings() const
{
	return settings;
}
