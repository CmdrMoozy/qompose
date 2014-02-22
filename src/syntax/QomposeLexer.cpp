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

#include "QomposeLexer.h"

/*!
 * This function returns the QomposeSettings key corresponding to the
 * style information for the given token.
 *
 * \param t The token to get a setting key for.
 * \return The setting key for the given token.
 */
QString QomposeLexer::getSettingKey(Token t)
{
	switch(t)
	{
		case PreprocessorToken:
			return "token-preprocessor";

		case CommentToken:
			return "token-comment";

		case StringToken:
			return "token-string";

		case KeywordToken:
			return "token-keyword";

		case OperatorToken:
			return "token-operator";

		case NumberToken:
			return "token-number";
	};

	return QString();
}

/*!
 * This is our default constructor, which creates a new instance of our lexer.
 *
 * \param p This object's parent object.
 */
QomposeLexer::QomposeLexer(QObject *p)
	: QObject(p)
{
}

/*!
 * This is our default destructor, which cleans up & destroys our object.
 */
QomposeLexer::~QomposeLexer()
{
}
