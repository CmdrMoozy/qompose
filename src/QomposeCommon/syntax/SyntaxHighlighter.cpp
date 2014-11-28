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

#include "SyntaxHighlighter.h"

#include <QObject>
#include <QTextDocument>
#include <QList>

#include "QomposeCommon/util/QomposeSettings.h"

namespace qompose
{

SyntaxHighlighter::SyntaxHighlighter(QomposeSettings *s, QObject *p)
	: QSyntaxHighlighter(p), settings(s), lexer(nullptr)
{
}

SyntaxHighlighter::SyntaxHighlighter(QomposeSettings *s, QTextDocument *p)
	: QSyntaxHighlighter(p), settings(s), lexer(nullptr)
{
}

SyntaxHighlighter::~SyntaxHighlighter()
{
}

QomposeSettings *SyntaxHighlighter::getSettings() const
{
	return settings;
}

void SyntaxHighlighter::setSettings(QomposeSettings *s)
{
	settings = s;

	rehighlight();
}

Lexer *SyntaxHighlighter::getLexer() const
{
	return lexer;
}

void SyntaxHighlighter::setLexer(Lexer *l)
{
	lexer = l;

	rehighlight();
}

void SyntaxHighlighter::highlightBlock(const QString &t)
{
	if(lexer == NULL)
	{
		#ifdef QOMPOSE_DEBUG
			qDebug("QomposeSyntaxHighlighter: Refusing to "
				"highlight with no valid lexer.");
		#endif

		return;
	}

	int state = -1;
	QList<LexerToken> tokens = lexer->lexBlock(
		&state, t, previousBlockState());

	for(int i = 0; i < tokens.size(); ++i)
	{
		setFormat(tokens.at(i).start, tokens.at(i).count,
			getFormatFor(tokens.at(i)));
	}

	if(state != currentBlockState())
		setCurrentBlockState(state);
}

QTextCharFormat SyntaxHighlighter::getFormatFor(const LexerToken &t)
{
	QString key = Lexer::getSettingKey(t.token);

	// TODO - Actually retrieve the proper format for this token from our settings instance.

	return QTextCharFormat();
}

}
