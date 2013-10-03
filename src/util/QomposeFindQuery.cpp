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

#include "QomposeFindQuery.h"

QomposeFindQuery::QomposeFindQuery(QObject *p)
	: QObject(p), expression(""), wrap(true), wholeWords(false),
		caseSensitive(false), reverse(false), regex(false)
{
}

QomposeFindQuery::~QomposeFindQuery()
{
}

QString QomposeFindQuery::getExpression() const
{
	return expression;
}

void QomposeFindQuery::setExpression(const QString &e)
{
	expression = e;
}

bool QomposeFindQuery::isWrapping() const
{
	return wrap;
}

void QomposeFindQuery::setWrapping(bool w)
{
	wrap = w;
}

bool QomposeFindQuery::isWholeWords() const
{
	return wholeWords;
}

void QomposeFindQuery::setWholeWords(bool w)
{
	wholeWords = w;
}

bool QomposeFindQuery::isCaseSensitive() const
{
	return caseSensitive;
}

void QomposeFindQuery::setCaseSensitive(bool c)
{
	caseSensitive = c;
}

bool QomposeFindQuery::isReversed() const
{
	return reverse;
}

void QomposeFindQuery::setReversed(bool r)
{
	reverse = r;
}

bool QomposeFindQuery::isRegularExpression() const
{
	return regex;
}

void QomposeFindQuery::setRegularExpression(bool r)
{
	regex = r;
}

QTextDocument::FindFlags QomposeFindQuery::getFindFlags(bool r) const
{
	QTextDocument::FindFlags f;
	
	if(r && isReversed())
		f |= QTextDocument::FindBackward;
	
	if(isCaseSensitive())
		f |= QTextDocument::FindCaseSensitively;
	
	if(isWholeWords())
		f |= QTextDocument::FindWholeWords;
	
	return f;
}
