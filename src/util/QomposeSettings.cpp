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

#include "QomposeSettings.h"

#include <QSettings>
#include <QStringList>
#include <QByteArray>
#include <QString>
#include <QFont>
#include <QColor>

// Load our default settings values into our static defaults list.

const QList< QPair<QString, QVariant> > QomposeSettings::defaults
	= (QList< QPair<QString, QVariant> >())
		<< QPair<QString, QVariant>( "show-status-bar",        true                  )
		<< QPair<QString, QVariant>( "recent-list-size",       static_cast<int>(10)  )
		<< QPair<QString, QVariant>( "recent-list",            QStringList()         )
		<< QPair<QString, QVariant>( "window-save-attributes", true                  )
		<< QPair<QString, QVariant>( "show-gutter",            true                  )
		<< QPair<QString, QVariant>( "editor-font",            QFont("Courier", 11)  )
		<< QPair<QString, QVariant>( "editor-tab-width",       static_cast<int>(8)   )
		<< QPair<QString, QVariant>( "editor-foreground",      QColor(255, 255, 255) )
		<< QPair<QString, QVariant>( "editor-background",      QColor(39, 40, 34)    )
		<< QPair<QString, QVariant>( "editor-current-line",    QColor(70, 72, 61)    )
		<< QPair<QString, QVariant>( "gutter-foreground",      QColor(Qt::white)     )
		<< QPair<QString, QVariant>( "gutter-background",      QColor(Qt::black)     )
		<< QPair<QString, QVariant>( "window-geometry",        QByteArray()          )
		<< QPair<QString, QVariant>( "window-state",           QByteArray()          );

/*!
 * This function initializes a new settings instance, with the given parent.
 * Note that, for settings which are currently unset, default values will
 * be automatically initialized.
 *
 * \param p Our parent object.
 */
QomposeSettings::QomposeSettings(QObject *p)
	: QObject(p)
{
	settings = new QSettings(QSettings::UserScope,
		"Qompose", "Qompose", this);
	
	initializeDefaults();
}

/*!
 * This is our default destructor, which cleans up & destroys our object.
 */
QomposeSettings::~QomposeSettings()
{
	delete settings;
}

/*!
 * This function returns the total number of settings keys our object is storing.
 *
 * \return The total number of settings keys.
 */
int QomposeSettings::count() const
{
	return settings->allKeys().count();
}

/*!
 * This function resets ALL of our settings to their default values. Any existing
 * data will be overwritten.
 */
void QomposeSettings::resetDefaults()
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
void QomposeSettings::resetDefault(const QString &k)
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
void QomposeSettings::setSetting(const QString &k, const QVariant &v)
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
bool QomposeSettings::containsSetting(const QString &k) const
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
QVariant QomposeSettings::getSetting(const QString &k) const
{
	return settings->value(k, QVariant());
}

/*!
 * This function initializes default values, for settings which have no value set
 * for them already. Existing values will NOT be overwritten - unlike resetDefaults().
 */
void QomposeSettings::initializeDefaults()
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
