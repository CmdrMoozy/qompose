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

#ifndef INCLUDE_QOMPOSECOMMON_GUI_STATUS_BAR_H
#define INCLUDE_QOMPOSECOMMON_GUI_STATUS_BAR_H

#include <string>

#include <QStatusBar>

#include "QomposeCommon/util/ConfigurationWatcher.hpp"

class QGridLayout;
class QLabel;
class QString;
class QVariant;

namespace qompose
{
class EllipsizedLabel;

/*!
 * \brief This class implements a status bar widget for our application.
 */
class StatusBar : public QStatusBar
{
	Q_OBJECT

public:
	/*!
	 * This is our default constructor, which creates a new instance of our
	 * status bar widget.
	 *
	 * \param p Our status bar's parent widget.
	 */
	StatusBar(QWidget *p = nullptr);

	StatusBar(const StatusBar &) = delete;
	virtual ~StatusBar() = default;

	StatusBar &operator=(const StatusBar &) = delete;

public Q_SLOTS:
	/*!
	 * This function sets the file path that we're displaying. This should
	 * be the absolute path to the file which is open, or an empty string
	 * if no valid absolute path exists.
	 *
	 * \param p The file path to display.
	 */
	void setFilePath(const QString &p);

	/*!
	 * \param l The new line number to display.
	 * \param c The new column number to display.
	 */
	void setCursorPosition(int l, int c);

private:
	qompose::util::ConfigurationWatcher *configWatcher;

	QWidget *statusWidget;
	QGridLayout *statusLayout;

	EllipsizedLabel *filePathLabel;
	QLabel *lineLabel;
	QLabel *columnLabel;

private Q_SLOTS:
	/*!
	 * This function handles a setting changed by applying that change.
	 *
	 * \param name The name of the setting which changed.
	 */
	void doSettingChanged(std::string const &name);
};
}

#endif
