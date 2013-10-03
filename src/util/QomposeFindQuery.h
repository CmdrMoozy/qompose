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

#ifndef INCLUDE_QOMPOSE_FIND_QUERY_H
#define INCLUDE_QOMPOSE_FIND_QUERY_H

#include <QObject>

#include <QString>
#include <QTextDocument>

class QomposeFindQuery : public QObject
{
	public:
		QomposeFindQuery(QObject *p = 0);
		virtual ~QomposeFindQuery();
		
		QString getExpression() const;
		void setExpression(const QString &e);
		
		bool isWrapping() const;
		void setWrapping(bool w);
		
		bool isWholeWords() const;
		void setWholeWords(bool w);
		
		bool isCaseSensitive() const;
		void setCaseSensitive(bool c);
		
		bool isReversed() const;
		void setReversed(bool r);
		
		bool isRegularExpression() const;
		void setRegularExpression(bool r);
		
		QTextDocument::FindFlags getFindFlags(bool r = false) const;
		
	private:
		QString expression;
		
		bool wrap;
		bool wholeWords;
		bool caseSensitive;
		bool reverse;
		bool regex;
};

#endif
