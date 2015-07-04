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

#include "Query.h"

namespace qompose
{
namespace editor
{
namespace search
{
FindQuery::FindQuery()
        : expression(""),
          wrap(false),
          wholeWords(false),
          caseSensitive(false),
          reverse(false),
          isRegex(false)
{
}

QTextDocument::FindFlags FindQuery::getFindFlags(bool includeReverse) const
{
	QTextDocument::FindFlags f = 0;
	if(includeReverse && reverse)
		f |= QTextDocument::FindBackward;
	if(caseSensitive)
		f |= QTextDocument::FindCaseSensitively;
	if(wholeWords)
		f |= QTextDocument::FindWholeWords;
	return f;
}

ReplaceQuery::ReplaceQuery() : FindQuery(), replaceValue("")
{
}
}
}
}
