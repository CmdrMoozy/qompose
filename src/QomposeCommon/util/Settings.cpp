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

const QList<QPair<QString, QVariant>> Settings::defaults =
        (QList<QPair<QString, QVariant>>())
        << SettingPair("show-file-in-title", true)
        << SettingPair("show-status-bar", true)
        << SettingPair("recent-list-size", 10)
        << SettingPair("recent-list", QStringList())
        << SettingPair("window-save-attributes", true)
        << SettingPair("show-gutter", true)
        << SettingPair("save-strip-trailing-spaces", true)
        << SettingPair("editor-font", QFont("Courier", 11))
        << SettingPair("editor-indentation-width", 8)
        << SettingPair("editor-indentation-mode", QString("tabs"))
        << SettingPair("editor-wrap-guide-visible", true)
        << SettingPair("editor-wrap-guide-width", 90)
        << SettingPair("editor-wrap-guide-color", QColor(127, 127, 127))
        << SettingPair("editor-foreground", QColor(255, 255, 255))
        << SettingPair("editor-background", QColor(39, 40, 34))
        << SettingPair("editor-current-line", QColor(70, 72, 61))
        << SettingPair("gutter-foreground", QColor(Qt::white))
        << SettingPair("gutter-background", QColor(Qt::black))
        << SettingPair("show-file-browser", false)
        << SettingPair("window-geometry", QByteArray())
        << SettingPair("window-state", QByteArray());

Settings::Settings(QObject *p) : QObject(p), settings(nullptr)
{
#ifdef QOMPOSE_DEBUG
	settings = new QSettings(QSettings::UserScope, "Qompose",
	                         "QomposeDebug", this);
#else
	settings =
	        new QSettings(QSettings::UserScope, "Qompose", "Qompose", this);
#endif

	initializeDefaults();
}

int Settings::count() const
{
	return settings->allKeys().count();
}

void Settings::resetDefaults()
{
	for(int i = 0; i < defaults.count(); ++i)
		setSetting(defaults.at(i).first, defaults.at(i).second);
}

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

void Settings::setSetting(const QString &k, const QVariant &v)
{
	settings->setValue(k, v);
	Q_EMIT settingChanged(k, v);
}

bool Settings::containsSetting(const QString &k) const
{
	return settings->contains(k);
}

QVariant Settings::getSetting(const QString &k) const
{
	return settings->value(k, QVariant());
}

void Settings::initializeDefaults()
{
	for(int i = 0; i < defaults.count(); ++i)
	{
		if(!containsSetting(defaults.at(i).first))
		{
			setSetting(defaults.at(i).first, defaults.at(i).second);
		}
	}
}
}
