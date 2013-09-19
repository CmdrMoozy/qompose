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

#include "QomposeFontPickerButton.h"

#include <QString>
#include <QIcon>
#include <QFontDialog>

/*!
 * This is one of our constructors, which intializes a new font picker
 * object with the given parameters.
 *
 * \param p Our parent widget.
 * \param iF Our initial font.
 */
QomposeFontPickerButton::QomposeFontPickerButton(QWidget *p, const QFont &iF)
	: QPushButton(tr("Select a Font"), p)
{
	setSelectedFont(iF);
	QObject::connect(this, SIGNAL(clicked()), this, SLOT(doClicked()));
}

/*!
 * This is one of our constructors, which intializes a new font picker object
 * with the given parameters.
 *
 * \param t Our button's text.
 * \param p Our parent widget.
 * \param iF Our initial font.
 */
QomposeFontPickerButton::QomposeFontPickerButton(const QString &t,
	QWidget *p, const QFont &iF)
	: QPushButton(t, p)
{
	setSelectedFont(iF);
	QObject::connect(this, SIGNAL(clicked()), this, SLOT(doClicked()));
}

/*!
 * This is one of our constructors, which intializes a new font picker
 * object with the given parameters.
 *
 * \param i Our button's icon.
 * \param t Our button's text.
 * \param p Our parent widget.
 * \param iF Our initial font.
 */
QomposeFontPickerButton::QomposeFontPickerButton(const QIcon &i,
	const QString &t, QWidget *p, const QFont &iF)
	: QPushButton(i, t, p)
{
	setSelectedFont(iF);
	QObject::connect(this, SIGNAL(clicked()), this, SLOT(doClicked()));
}

/*!
 * This is our default destructor, which cleans up and destroys our object.
 */
QomposeFontPickerButton::~QomposeFontPickerButton()
{
}

/*!
 * This function simply returns the font we are currently representing.
 * Note that this is NOT necessarily the font we are displaying (i.e.,
 * the value returned by font()).
 *
 * \return Our currently selected font.
 */
const QFont &QomposeFontPickerButton::getSelectedFont() const
{
	return selectedFont;
}

/*!
 * This function allows the currently selected font to be set. Note that
 * this will NOT necessarily become the font we are displaying.
 *
 * Note that this function does NOT emit a selectedFontChanged() signal -
 * that is reserved for the font being changed by the USER when they click us.
 *
 * \param f The new font to represent.
 */
void QomposeFontPickerButton::setSelectedFont(const QFont &f)
{
	QFont display = selectedFont = f;
	
	if(display.pointSize() > -1)
		display.setPointSize(qMin(display.pointSize(), 24));
	else
		display.setPixelSize(qMin(display.pixelSize(), 50));
	
	setFont(display);
}

/*!
 * We override our parent class's implementation of this, because we
 * will be handling what font we are using internally; the user should
 * be calling setSelectedFont() if they want to change the font we are
 * currently representing.
 *
 * \param f The font we will display.
 */
void QomposeFontPickerButton::setFont(const QFont &f)
{
	QPushButton::setFont(f);
}

/*!
 * When we are clicked, we want to automatically pop up a QFontDialog
 * to get a new font selection from the user. If the user selects a
 * new font (i.e., doesn't click "Cancel"), then we emit a
 * selectedFontChanged() signal.
 */
void QomposeFontPickerButton::doClicked()
{ /* SLOT */
	bool ok;
	setSelectedFont(QFontDialog::getFont(&ok, getSelectedFont(), this, tr("Select a Font")));
	
	if(ok)
		emit selectedFontChanged(getSelectedFont());
}

