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

#ifndef INCLUDE_QOMPOSECOMMON_SYNTAX_LEXER_H
#define INCLUDE_QOMPOSECOMMON_SYNTAX_LEXER_H

#include <QList>
#include <QObject>

namespace qompose
{
// Forward declarations.

struct LexerToken;
typedef struct LexerToken LexerToken;

/*!
 * \brief This class provides a lexer interface for syntax highlighting lexers.
 */
class Lexer : public QObject
{
public:
	/*!
	 * \brief Defines the states a block can end in after being lex'ed.
	 */
	enum BlockState
	{
		UninitializedState = 0,
		NormalState = 1,
		PreprocessorState = 2,
		CommentState = 4,
		StringState = 8
	};

	/*!
	 * \brief Defines the different types of tokens our lexer looks for.
	 */
	enum Token
	{
		PreprocessorToken = 0,
		CommentToken = 1,
		StringToken = 2,
		KeywordToken = 3,
		OperatorToken = 4,
		NumberToken = 5
	};

	static QString getSettingKey(Token);

	Lexer(QObject * = nullptr);
	virtual ~Lexer();

	virtual QList<LexerToken> lexBlock(int *, const QString &, int) = 0;
};

/*!
 * \brief This struct denotes a single token generated by our lexer.
 *
 * The start is the offset of the first character of this token in the block
 * that was lex'ed (NOT relative to the start of the document), and the count
 * is the length of this token in characters.
 */
struct LexerToken
{
	int start;
	int count;
	Lexer::Token token;
};
}

#endif
