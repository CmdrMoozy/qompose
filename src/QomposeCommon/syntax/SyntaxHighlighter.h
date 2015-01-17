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

#ifndef INCLUDE_QOMPOSECOMMON_SYNTAX_SYNTAX_HIGHLIGHTER_H
#define INCLUDE_QOMPOSECOMMON_SYNTAX_SYNTAX_HIGHLIGHTER_H

#include <QSyntaxHighlighter>

#include <QTextCharFormat>

#include "QomposeCommon/syntax/Lexer.h"

class QObject;
class QTextDocument;

namespace qompose
{
class Settings;

class SyntaxHighlighter : public QSyntaxHighlighter
{
public:
	SyntaxHighlighter(Settings *, QObject *);
	SyntaxHighlighter(Settings *, QTextDocument *);
	virtual ~SyntaxHighlighter();

	Settings *getSettings() const;
	void setSettings(Settings *);

	Lexer *getLexer() const;
	void setLexer(Lexer *);

protected:
	virtual void highlightBlock(const QString &);

private:
	Settings *settings;
	Lexer *lexer;

	SyntaxHighlighter(const SyntaxHighlighter &);
	SyntaxHighlighter &operator=(const SyntaxHighlighter &);

	QTextCharFormat getFormatFor(const LexerToken &);
};
}

#endif
