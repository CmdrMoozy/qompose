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

#include "QomposeColorPickerButton.h"

#include <QString>
#include <QIcon>
#include <QColorDialog>
#include <QPaintEvent>
#include <QPainter>
#include <QRect>

#include "QomposeDefines.h"

/*!
 * This is one of our constructors, which intializes a new color picker
 * object with the given parameters.
 *
 * \param p Our parent widget.
 * \param iC Our initial color.
 */
QomposeColorPickerButton::QomposeColorPickerButton(QWidget *p, const QColor &iC)
	: QPushButton("", p)
{
	setSelectedColor(iC);
	setMinimumSize(75, minimumHeight());

	QObject::connect(this, SIGNAL(clicked()), this, SLOT(doClicked()));
}

/*!
 * This is one of our constructors, which intializes a new color picker object
 * with the given parameters.
 *
 * \param t Our button's text (IGNORED).
 * \param p Our parent widget.
 * \param iC Our initial color.
 */
QomposeColorPickerButton::QomposeColorPickerButton(const QString &QUNUSED(t),
	QWidget *p, const QColor &iC)
	: QPushButton("", p)
{
	setSelectedColor(iC);
	setMinimumSize(75, minimumHeight());

	QObject::connect(this, SIGNAL(clicked()), this, SLOT(doClicked()));
}

/*!
 * This is one of our constructors, which intializes a new color picker
 * object with the given parameters.
 *
 * \param i Our button's icon (IGNORED).
 * \param t Our button's text (IGNORED).
 * \param p Our parent widget.
 * \param iC Our initial color.
 */
QomposeColorPickerButton::QomposeColorPickerButton(const QIcon &QUNUSED(i),
	const QString &QUNUSED(t), QWidget *p, const QColor &iC)
	: QPushButton("", p)
{
	setSelectedColor(iC);
	setMinimumSize(75, minimumHeight());

	QObject::connect(this, SIGNAL(clicked()), this, SLOT(doClicked()));
}

/*!
 * This is our default constructor, which cleans up and destroys our object.
 */
QomposeColorPickerButton::~QomposeColorPickerButton()
{
}

/*!
 * This function simply returns the color we are currently representing. This
 * is also the color that is displayed by our button.
 *
 * \return Our currently selected color.
 */
const QColor &QomposeColorPickerButton::getSelectedColor() const
{
	return selectedColor;
}

/*!
 * This function allows the currently selected color to be set. This will be the
 * color that our button displays.
 *
 * Note that this function does NOT emit a selectedColorChanged() signal - that
 * is reserved for the color being changed by the USER when they click us.
 *
 * \param c The new color to represent.
 */
void QomposeColorPickerButton::setSelectedColor(const QColor &c)
{
	selectedColor = c;
	update();
}

/*!
 * We override our parent class's paint event so we can draw the color we are
 * representing on our button.
 *
 * \param e The event we are handling.
 */
void QomposeColorPickerButton::paintEvent(QPaintEvent *e)
{
	// Call our superclass' paint event so we still look like a button.
	QPushButton::paintEvent(e);

	// Create a new painter for us.
	QPainter painter(this);

	// Figure out where we are going to paint - leave a margin for our buttony parts.

	QRect r = rect();

	#ifdef _WIN32
		r.adjust(5, 5, -5, -7);
	#else
		#ifdef __APPLE__
			r.adjust(9, 7, -9, -10);
		#else
			r.adjust(4, 4, -4, -5);
		#endif
	#endif

	// Fill our rectangle.
	painter.fillRect(r, getSelectedColor());
}

/*!
 * We override our parent class's setIcon() function, since this particular button
 * does not, by definition, display icons. Note that this function still performs its
 * function correctly, but is private.
 *
 * \param i The new icon to display.
 */
void QomposeColorPickerButton::setIcon(const QIcon &i)
{
	QPushButton::setIcon(i);
}

/*!
 * We override our parent class's setText() function, since this particular button
 * does not, by definition, display text. Note that this function still performs its
 * function correctly, but is private.
 *
 * \param i The new text to display.
 */
void QomposeColorPickerButton::setText(const QString &t)
{
	QPushButton::setText(t);
}

/*!
 * When we are clicked, we want to automatically pop up a QColorDialog to get a new
 * color selection from the user. If the user selects a new color (i.e., doesn't
 * click "Cancel"), then we emit a selectedColorChanged() signal.
 */
void QomposeColorPickerButton::doClicked()
{ /* SLOT */

	QColor c = QColorDialog::getColor(getSelectedColor(), this,
		tr("Select a Color"), QColorDialog::DontUseNativeDialog);

	if(c.isValid())
	{
		c.setAlpha(255); // Make sure our alpha is normal, as it isn't selectable.
		setSelectedColor(c);
		Q_EMIT selectedColorChanged(getSelectedColor());
	}

}

