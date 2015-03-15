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

#ifndef INCLUDE_QOMPOSECOMMON_UTIL_REPLACE_QUERY_H
#define INCLUDE_QOMPOSECOMMON_UTIL_REPLACE_QUERY_H

#include "QomposeCommon/util/FindQuery.h"

#include <QString>

namespace qompose
{
/*!
 * \brief This class extends our find query object by adding replace details.
 */
class ReplaceQuery : public FindQuery
{
public:
	/*!
	 * This is our default constructor, which creates a new replace query.
	 *
	 * \param p The parent object for this replace query.
	 */
	ReplaceQuery(QObject *p = nullptr);

	ReplaceQuery(const ReplaceQuery &) = delete;
	virtual ~ReplaceQuery() = default;

	ReplaceQuery &operator=(const ReplaceQuery &) = delete;

	/*!
	 * This function returns the replace value our replace query is
	 * currently going to be replacing matches with.
	 *
	 * \return Our current replacement value.
	 */
	QString getReplaceValue() const;

	/*!
	 * This function sets the replace value our replace query will use to
	 * replace matches with.
	 *
	 * \param v The new replacement value.
	 */
	void setReplaceValue(const QString &v);

private:
	QString replaceValue;
};
}

#endif
