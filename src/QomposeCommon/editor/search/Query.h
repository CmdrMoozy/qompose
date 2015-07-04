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

#ifndef INCLUDE_QOMPOSECOMMON_EDITOR_SEARCH_QUERY_H
#define INCLUDE_QOMPOSECOMMON_EDITOR_SEARCH_QUERY_H

#include <QString>
#include <QTextDocument>

namespace qompose
{
namespace editor
{
namespace search
{
struct FindQuery
{
	QString expression;
	bool wrap;
	bool wholeWords;
	bool caseSensitive;
	bool reverse;
	bool isRegex;

	FindQuery();
	FindQuery(const FindQuery &) = default;
	~FindQuery() = default;
	FindQuery &operator=(const FindQuery &) = default;

	QTextDocument::FindFlags
	getFindFlags(bool includeReverse = false) const;
};

struct ReplaceQuery : public FindQuery
{
	QString replaceValue;

	ReplaceQuery();
};
}
}
}

#endif
