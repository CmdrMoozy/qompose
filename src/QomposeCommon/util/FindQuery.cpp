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

#include "FindQuery.h"

namespace qompose
{
FindQuery::FindQuery(QObject *p)
        : QObject(p),
          expression(""),
          wrap(true),
          wholeWords(false),
          caseSensitive(false),
          reverse(false),
          regex(false)
{
}

QString FindQuery::getExpression() const
{
	return expression;
}

void FindQuery::setExpression(const QString &e)
{
	expression = e;
}

bool FindQuery::isWrapping() const
{
	return wrap;
}

void FindQuery::setWrapping(bool w)
{
	wrap = w;
}

bool FindQuery::isWholeWords() const
{
	return wholeWords;
}

void FindQuery::setWholeWords(bool w)
{
	wholeWords = w;
}

bool FindQuery::isCaseSensitive() const
{
	return caseSensitive;
}

void FindQuery::setCaseSensitive(bool c)
{
	caseSensitive = c;
}

bool FindQuery::isReversed() const
{
	return reverse;
}

void FindQuery::setReversed(bool r)
{
	reverse = r;
}

bool FindQuery::isRegularExpression() const
{
	return regex;
}

void FindQuery::setRegularExpression(bool r)
{
	regex = r;
}

QTextDocument::FindFlags FindQuery::getFindFlags(bool r) const
{
	QTextDocument::FindFlags f = nullptr;

	if(r && isReversed())
		f |= QTextDocument::FindBackward;

	if(isCaseSensitive())
		f |= QTextDocument::FindCaseSensitively;

	if(isWholeWords())
		f |= QTextDocument::FindWholeWords;

	return f;
}
}
