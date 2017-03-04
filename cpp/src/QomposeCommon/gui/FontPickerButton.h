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

#ifndef INCLUDE_QOMPOSECOMMON_GUI_FONT_PICKER_BUTTON_H
#define INCLUDE_QOMPOSECOMMON_GUI_FONT_PICKER_BUTTON_H

#include <QFont>
#include <QPushButton>

class QString;
class QIcon;

namespace qompose
{
/*!
 * \brief This class implements a QPushButton for selecting a font.
 *
 * When clicked, this button automatically prompts the user for a new font. To
 * demonstrate the font selected, we set our widget's font to the one the user
 * chose. Note that, to preserve layouts, we do limit the font size we will
 * display -- but any size of font can be selected, and will be returned by
 * getSelectedFont() appropriately.
 */
class FontPickerButton : public QPushButton
{
	Q_OBJECT

public:
	/*!
	 * This is one of our constructors, which intializes a new font picker
	 * object with the given parameters.
	 *
	 * \param p Our parent widget.
	 * \param iF Our initial font.
	 */
	FontPickerButton(QWidget *p = nullptr, const QFont &iF = QFont());

	/*!
	 * This is one of our constructors, which intializes a new font picker
	 * object with the given parameters.
	 *
	 * \param t Our button's text.
	 * \param p Our parent widget.
	 * \param iF Our initial font.
	 */
	FontPickerButton(const QString &t, QWidget *p = nullptr,
	                 const QFont &iF = QFont());

	/*!
	 * This is one of our constructors, which intializes a new font picker
	 * object with the given parameters.
	 *
	 * \param i Our button's icon.
	 * \param t Our button's text.
	 * \param p Our parent widget.
	 * \param iF Our initial font.
	 */
	FontPickerButton(const QIcon &i, const QString &t, QWidget *p = nullptr,
	                 const QFont &iF = QFont());

	FontPickerButton(const FontPickerButton &) = delete;
	virtual ~FontPickerButton() = default;

	FontPickerButton &operator=(const FontPickerButton &) = delete;

	/*!
	 * This function simply returns the font we are currently representing.
	 * Note that this is NOT necessarily the font we are displaying (i.e.,
	 * the value returned by font()).
	 *
	 * \return Our currently selected font.
	 */
	const QFont &getSelectedFont() const;

	/*!
	 * This function allows the currently selected font to be set. Note
	 * that this will NOT necessarily become the font we are displaying.
	 *
	 * Note that this function does NOT emit a selectedFontChanged()
	 * signal - that is reserved for the font being changed by the USER
	 * when they click us.
	 *
	 * \param f The new font to represent.
	 */
	void setSelectedFont(const QFont &f);

private:
	QFont selectedFont;

	/*!
	 * We override our parent class's implementation of this, because we
	 * will be handling what font we are using internally; the user should
	 * be calling setSelectedFont() if they want to change the font we are
	 * currently representing.
	 *
	 * \param f The font we will display.
	 */
	virtual void setFont(const QFont &f);

private Q_SLOTS:
	/*!
	 * When we are clicked, we want to automatically pop up a QFontDialog
	 * to get a new font selection from the user. If the user selects a
	 * new font (i.e., doesn't click "Cancel"), then we emit a
	 * selectedFontChanged() signal.
	 */
	void doClicked();

Q_SIGNALS:
	void selectedFontChanged(const QFont &);
};
}

#endif
