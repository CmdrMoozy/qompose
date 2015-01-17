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

#ifndef INCLUDE_QOMPOSECOMMON_UTIL_FIND_QUERY_H
#define INCLUDE_QOMPOSECOMMON_UTIL_FIND_QUERY_H

#include <QObject>

#include <QString>
#include <QTextDocument>

namespace qompose
{
/*!
 * \brief This class encapsulates the options and expression of a find query.
 */
class FindQuery : public QObject
{
public:
	FindQuery(QObject * = nullptr);
	virtual ~FindQuery();

	QString getExpression() const;
	void setExpression(const QString &);

	bool isWrapping() const;
	void setWrapping(bool);

	bool isWholeWords() const;
	void setWholeWords(bool);

	bool isCaseSensitive() const;
	void setCaseSensitive(bool);

	bool isReversed() const;
	void setReversed(bool);

	bool isRegularExpression() const;
	void setRegularExpression(bool);

	QTextDocument::FindFlags getFindFlags(bool = false) const;

private:
	QString expression;

	bool wrap;
	bool wholeWords;
	bool caseSensitive;
	bool reverse;
	bool regex;
};
}

#endif
