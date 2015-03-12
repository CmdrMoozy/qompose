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

#ifndef INCLUDE_QOMPOSECOMMON_SPELL_SPELLING_UTILS_H
#define INCLUDE_QOMPOSECOMMON_SPELL_SPELLING_UTILS_H

#include <QString>
#include <QLocale>

namespace qompose
{
namespace spelling_utils
{
/*!
 * This function returns the ISO-1 language code for the given QLocale's
 * language() property. If the language property value is not supported, then
 * a default-constructed QString will be returned instead.
 *
 * For more information, see:
 *
 *     https://en.wikipedia.org/wiki/List_of_ISO_639-1_codes
 *
 * \param locale The locale object to inspect.
 * \return The ISO-1 language code for the locale's language.
 */
QString getLocaleLanguageCode(const QLocale &locale);

/*!
 * This function returns the ISO 3166-1 alpha-2 country code for the given
 * QLocale's country() property. If the country property value is not
 * supported, then a default-constructed QString will be returned instead.
 *
 * For more information, see:
 *
 *     https://en.wikipedia.org/wiki/ISO_3166-1_alpha-2
 *
 * \param locale The locale object to inspect.
 * \return The ISO 3166-1 alpha-2 country code for the locale's country.
 */
QString getLocaleCountryCode(const QLocale &locale);
}
}

#endif
