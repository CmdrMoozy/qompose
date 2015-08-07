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

#include <stdexcept>
#include <utility>
#include <vector>

#include <QByteArray>
#include <QColor>
#include <QFont>
#include <QSettings>
#include <QString>
#include <QStringList>
#include <QVariant>

namespace
{
const std::vector<std::pair<QString, QVariant>> DEFAULT_SETTINGS = {
        {"show-file-in-title", true},
        {"show-status-bar", true},
        {"recent-list-size", 10},
        {"recent-list", QStringList()},
        {"window-save-attributes", true},
        {"show-gutter", true},
        {"save-strip-trailing-spaces", true},
        {"editor-font", QFont("Courier", 11)},
        {"editor-indentation-width", 8},
        {"editor-indentation-mode", QString("tabs")},
        {"editor-wrap-guide-visible", true},
        {"editor-wrap-guide-width", 90},
        {"editor-wrap-guide-color", QColor(127, 127, 127)},
        {"editor-foreground", QColor(255, 255, 255)},
        {"editor-background", QColor(39, 40, 34)},
        {"editor-current-line", QColor(70, 72, 61)},
        {"gutter-foreground", QColor(Qt::white)},
        {"gutter-background", QColor(Qt::black)},
        {"show-file-browser", false},
        {"window-geometry", QByteArray()},
        {"window-state", QByteArray()}};
}

namespace qompose
{
std::mutex Settings::mutex;
std::unique_ptr<Settings> Settings::settingsSingleton;

void Settings::initialize()
{
	std::lock_guard<std::mutex> lock(mutex);
	if(!!settingsSingleton)
	{
		throw std::runtime_error(
		        "Settings singleton already initialized.");
	}
	settingsSingleton.reset(new Settings());
}

void Settings::destroy()
{
	std::lock_guard<std::mutex> lock(mutex);
	settingsSingleton.reset();
}

Settings &Settings::instance()
{
	std::lock_guard<std::mutex> lock(mutex);
	if(!settingsSingleton)
	{
		throw std::runtime_error("Settings singleton not initialized.");
	}
	return *settingsSingleton;
}

void Settings::resetDefaults()
{
	for(const auto &pair : DEFAULT_SETTINGS)
		setSetting(pair.first, pair.second);
}

void Settings::resetDefault(const QString &k)
{
	for(const auto &pair : DEFAULT_SETTINGS)
	{
		if(pair.first == k)
		{
			setSetting(k, pair.second);
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

void Settings::initializeDefaults()
{
	for(const auto &pair : DEFAULT_SETTINGS)
	{
		if(!containsSetting(pair.first))
			setSetting(pair.first, pair.second);
	}
}
}
