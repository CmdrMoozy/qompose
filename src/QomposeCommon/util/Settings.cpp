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

#include "Settings.h"

#include <QSettings>
#include <QStringList>
#include <QByteArray>
#include <QString>
#include <QFont>
#include <QColor>

namespace
{

typedef QPair<QString, QVariant> SettingPair;

}

namespace qompose
{

// Load our default settings values into our static defaults list.

const QList< QPair<QString, QVariant> > Settings::defaults =
	(QList< QPair<QString, QVariant> >())
	<< SettingPair("show-file-in-title", true)
	<< SettingPair("show-status-bar", true)
	<< SettingPair("recent-list-size", 10)
	<< SettingPair("recent-list", QStringList())
	<< SettingPair("window-save-attributes", true)
	<< SettingPair("show-gutter", true)
	<< SettingPair("save-strip-trailing-spaces", true)
	<< SettingPair("editor-font", QFont("Courier", 11))
	<< SettingPair("editor-indentation-width", 8)
	<< SettingPair("editor-wrap-guide-visible", true)
	<< SettingPair("editor-wrap-guide-width", 90)
	<< SettingPair("editor-wrap-guide-color", QColor(127, 127, 127))
	<< SettingPair("editor-foreground", QColor(255, 255, 255))
	<< SettingPair("editor-background", QColor(39, 40, 34))
	<< SettingPair("editor-current-line", QColor(70, 72, 61))
	<< SettingPair("gutter-foreground", QColor(Qt::white))
	<< SettingPair("gutter-background", QColor(Qt::black))
	<< SettingPair("window-geometry", QByteArray())
	<< SettingPair("window-state", QByteArray());

/*!
 * This function initializes a new settings instance, with the given parent.
 * Note that, for settings which are currently unset, default values will
 * be automatically initialized.
 *
 * \param p Our parent object.
 */
Settings::Settings(QObject *p)
	: QObject(p), settings(nullptr)
{
	#ifdef QOMPOSE_DEBUG
		settings = new QSettings(QSettings::UserScope,
			"Qompose", "QomposeDebug", this);
	#else
		settings = new QSettings(QSettings::UserScope,
			"Qompose", "Qompose", this);
	#endif

	initializeDefaults();
}

/*!
 * This is our default destructor, which cleans up & destroys our object.
 */
Settings::~Settings()
{
	delete settings;
}

/*!
 * This function returns the total number of settings keys our object is
 * storing.
 *
 * \return The total number of settings keys.
 */
int Settings::count() const
{
	return settings->allKeys().count();
}

/*!
 * This function resets ALL of our settings to their default values. Any
 * existing data will be overwritten.
 */
void Settings::resetDefaults()
{
	for(int i = 0; i < defaults.count(); ++i)
		setSetting(defaults.at(i).first, defaults.at(i).second);
}

/*!
 * This function resets one particular setting to its default value. Any
 * existing data will be overwritten.
 *
 * \param k The key of the setting to reset.
 */
void Settings::resetDefault(const QString &k)
{
	for(int i = 0; i < defaults.count(); ++i)
	{
		if(defaults.at(i).first == k)
		{
			setSetting(k, defaults.at(i).second);
			break;
		}
	}
}

/*!
 * This function sets the given setting key to the given value.
 *
 * \param k The setting key to set.
 * \param v The new value for the given key.
 */
void Settings::setSetting(const QString &k, const QVariant &v)
{
	settings->setValue(k, v);
	Q_EMIT settingChanged(k, v);
}

/*!
 * This function returns whether or not our object contains a value for the
 * given settings key.
 *
 * \param k The key to search for.
 * \return True if we have a value for the given key, or false otherwise.
 */
bool Settings::containsSetting(const QString &k) const
{
	return settings->contains(k);
}

/*!
 * This function retrieves the value associated with the given settings key. If
 * the given key is not present, then we will return QVariant() instead.
 *
 * \param k The key to search for.
 * \return The value for the given settings key.
 */
QVariant Settings::getSetting(const QString &k) const
{
	return settings->value(k, QVariant());
}

/*!
 * This function initializes default values, for settings which have no value
 * set for them already. Existing values will NOT be overwritten - unlike
 * resetDefaults().
 */
void Settings::initializeDefaults()
{
	for(int i = 0; i < defaults.count(); ++i)
	{
		if(!containsSetting(defaults.at(i).first))
		{
			setSetting(defaults.at(i).first,
				defaults.at(i).second);
		}
	}
}

}
