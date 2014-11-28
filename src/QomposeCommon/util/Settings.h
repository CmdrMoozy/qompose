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

#ifndef INCLUDE_QOMPOSECOMMON_UTIL_SETTINGS_H
#define INCLUDE_QOMPOSECOMMON_UTIL_SETTINGS_H

#include <QObject>
#include <QPair>
#include <QList>

class QSettings;
class QString;

namespace qompose
{

/*!
 * \brief This class uses QSettings to provide persistent settings for Qompose.
 */
class Settings : public QObject
{
	Q_OBJECT

public:
	Settings(QObject * = nullptr);
	virtual ~Settings();

	int count() const;

	void resetDefaults();
	void resetDefault(const QString &);

	void setSetting(const QString &, const QVariant &);
	bool containsSetting(const QString &) const;
	QVariant getSetting(const QString &) const;

private:
	static const QList<QPair<QString, QVariant>> defaults;
	QSettings *settings;

	Settings(const Settings &);
	Settings &operator=(const Settings &);

	void initializeDefaults();

Q_SIGNALS:
	void settingChanged(const QString &, const QVariant &);
};

}

#endif
