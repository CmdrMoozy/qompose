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
	/*!
	 * This function initializes a new settings instance, with the given
	 * parent. Note that, for settings which are currently unset, default
	 * values will be automatically initialized.
	 *
	 * \param p Our parent object.
	 */
	Settings(QObject *p = nullptr);

	Settings(const Settings &) = delete;
	virtual ~Settings() = default;

	Settings &operator=(const Settings &) = delete;

	/*!
	 * This function returns the total number of settings keys our object
	 * is storing.
	 *
	 * \return The total number of settings keys.
	 */
	int count() const;

	/*!
	 * This function resets ALL of our settings to their default values.
	 * Any existing data will be overwritten.
	 */
	void resetDefaults();

	/*!
	 * This function resets one particular setting to its default value.
	 * Any existing data will be overwritten.
	 *
	 * \param k The key of the setting to reset.
	 */
	void resetDefault(const QString &k);

	/*!
	 * This function sets the given setting key to the given value.
	 *
	 * \param k The setting key to set.
	 * \param v The new value for the given key.
	 */
	void setSetting(const QString &k, const QVariant &v);

	/*!
	 * This function returns whether or not our object contains a value for
	 * the given settings key.
	 *
	 * \param k The key to search for.
	 * \return True if we have a value for the given key, or false.
	 */
	bool containsSetting(const QString &k) const;

	/*!
	 * This function retrieves the value associated with the given settings
	 * key. If the given key is not present, then we will return QVariant()
	 * instead.
	 *
	 * \param k The key to search for.
	 * \return The value for the given settings key.
	 */
	QVariant getSetting(const QString &k) const;

private:
	static const QList<QPair<QString, QVariant>> defaults;
	QSettings *settings;

	/*!
	 * This function initializes default values, for settings which have no
	 * value set for them already. Existing values will NOT be overwritten
	 * (unlike resetDefaults()).
	 */
	void initializeDefaults();

Q_SIGNALS:
	void settingChanged(const QString &, const QVariant &);
};
}

#endif
