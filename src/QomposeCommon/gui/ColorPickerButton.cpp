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

#include "ColorPickerButton.h"

#include <QColorDialog>
#include <QIcon>
#include <QPaintEvent>
#include <QPainter>
#include <QRect>
#include <QString>

#include "QomposeCommon/Defines.h"

namespace qompose
{
ColorPickerButton::ColorPickerButton(QWidget *p, const QColor &iC)
        : QPushButton("", p), selectedColor(QColor())
{
	setSelectedColor(iC);
	setMinimumSize(75, minimumHeight());

	QObject::connect(this, SIGNAL(clicked()), this, SLOT(doClicked()));
}

ColorPickerButton::ColorPickerButton(const QString &QUNUSED(t), QWidget *p,
                                     const QColor &iC)
        : QPushButton("", p), selectedColor(QColor())
{
	setSelectedColor(iC);
	setMinimumSize(75, minimumHeight());

	QObject::connect(this, SIGNAL(clicked()), this, SLOT(doClicked()));
}

ColorPickerButton::ColorPickerButton(const QIcon &QUNUSED(i),
                                     const QString &QUNUSED(t), QWidget *p,
                                     const QColor &iC)
        : QPushButton("", p), selectedColor(QColor())
{
	setSelectedColor(iC);
	setMinimumSize(75, minimumHeight());

	QObject::connect(this, SIGNAL(clicked()), this, SLOT(doClicked()));
}

const QColor &ColorPickerButton::getSelectedColor() const
{
	return selectedColor;
}

void ColorPickerButton::setSelectedColor(const QColor &c)
{
	selectedColor = c;
	update();
}

void ColorPickerButton::paintEvent(QPaintEvent *e)
{
	// Call our superclass' paint event so we still look like a button.
	QPushButton::paintEvent(e);

	// Create a new painter for us.
	QPainter painter(this);

	// Leave a margin for our buttony parts when painting.

	QRect r = rect();

#if defined(_WIN32)
	r.adjust(5, 5, -5, -7);
#elif defined(__APPLE__)
	r.adjust(9, 7, -9, -10);
#else
	r.adjust(4, 4, -4, -5);
#endif

	// Fill our rectangle.
	painter.fillRect(r, getSelectedColor());
}

void ColorPickerButton::setIcon(const QIcon &i)
{
	QPushButton::setIcon(i);
}

void ColorPickerButton::setText(const QString &t)
{
	QPushButton::setText(t);
}

void ColorPickerButton::doClicked()
{
	QColor c = QColorDialog::getColor(getSelectedColor(), this,
	                                  tr("Select a Color"),
	                                  QColorDialog::DontUseNativeDialog);

	if(c.isValid())
	{
		// Make sure our alpha is normal, as it isn't selectable.
		c.setAlpha(255);

		setSelectedColor(c);
		Q_EMIT selectedColorChanged(getSelectedColor());
	}
}
}
