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

/*!
 * This is our default constructor, which creates a new find query object
 * with the default option values.
 *
 * \param p This object's parent object.
 */
FindQuery::FindQuery(QObject *p)
	: QObject(p), expression(""), wrap(true), wholeWords(false),
		caseSensitive(false), reverse(false), regex(false)
{
}

/*!
 * This is our default destructor, which cleans up & destroys our object.
 */
FindQuery::~FindQuery()
{
}

/*!
 * This function returns, as a string, our object's current find expression.
 * This can be either a flat string, or a regular expression, depending on
 * our other options.
 *
 * \return Our object's current find expression.
 */
QString FindQuery::getExpression() const
{
	return expression;
}

/*!
 * This function sets our object's find expression. This expression can be
 * either a flat string or a regular expression, depending on the value of
 * the isRegularExpression() option.
 *
 * \param e The new expression to use.
 */
void FindQuery::setExpression(const QString &e)
{
	expression = e;
}

/*!
 * This function returns whether or not wrapping is enabled for this find
 * query. That is, whether or not our caller should wrap back around the end
 * of the document after all matches have been exhausted.
 *
 * \return Whether or not wrapping is currently enabled.
 */
bool FindQuery::isWrapping() const
{
	return wrap;
}

/*!
 * This function sets whether or not wrapping is enabled for this find query.
 * That is, whether or not our caller should wrap back around the end of
 * the document after all matches have been exhausted.
 *
 * \param w The new wrapping setting to use.
 */
void FindQuery::setWrapping(bool w)
{
	wrap = w;
}

/*!
 * This function returns whether or not this find query will only match whole
 * words.
 *
 * \return Whether or not our query will only match whole words currently.
 */
bool FindQuery::isWholeWords() const
{
	return wholeWords;
}

/*!
 * This function sets whether or not whole words matching is required for this
 * find query.
 *
 * \param w The new whole words matching setting to use.
 */
void FindQuery::setWholeWords(bool w)
{
	wholeWords = w;
}

/*!
 * This function returns whether or not our find query is case sensitive.
 *
 * \return Whether or not our query is case sensitive currently.
 */
bool FindQuery::isCaseSensitive() const
{
	return caseSensitive;
}

/*!
 * This function sets whether or not our find query should be case sensitive.
 *
 * \param c The new case sensitive setting to use.
 */
void FindQuery::setCaseSensitive(bool c)
{
	caseSensitive = c;
}

/*!
 * This function returns whether or not our find query's direction should be
 * reversed. Note that this does not guarantee one direction or the other,
 * since "find next" and "find previous" have different default directions.
 *
 * \return Whether or not our query is currently searching in reverse order.
 */
bool FindQuery::isReversed() const
{
	return reverse;
}

/*!
 * This function sets whether or not our find query's direction should be
 * reversed. Note that this does not guarantee one direction or the other,
 * since "find next" and "find previous" have different default directions.
 *
 * \param r The new reversed direction setting to use.
 */
void FindQuery::setReversed(bool r)
{
	reverse = r;
}

/*!
 * This function returns whether or not our find expression is a regular
 * expression.
 *
 * \return Whether or not our query is currently a regular expression.
 */
bool FindQuery::isRegularExpression() const
{
	return regex;
}

/*!
 * This function sets whether or not our query's find expression is a regular
 * expression.
 *
 * \param r The new regular expression setting to use.
 */
void FindQuery::setRegularExpression(bool r)
{
	regex = r;
}

/*!
 * This function encodes our find query options as a QTextDocument::FindFlags
 * value, optionally including the "reversed" option in the resulting flags.
 *
 * The direction of a search is dependant on whether the operation is a "next"
 * or "previous" operation, as well as the reversed flag, so it may be valuable
 * for the user of this class to exclude the "reversed" flag from the end set
 * of find flags, instead determining direction separately.
 *
 * \param r Whether or not the "reversed" option should be added to the flags.
 * \return The FindFlags value for our current set of find query options.
 */
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
