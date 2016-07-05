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

#include "FontPickerButton.h"

#include <QFontDialog>
#include <QIcon>
#include <QString>

namespace qompose
{
FontPickerButton::FontPickerButton(QWidget *p, const QFont &iF)
        : QPushButton(tr("Select a Font"), p), selectedFont(QFont())
{
	setSelectedFont(iF);
	QObject::connect(this, SIGNAL(clicked()), this, SLOT(doClicked()));
}

FontPickerButton::FontPickerButton(const QString &t, QWidget *p,
                                   const QFont &iF)
        : QPushButton(t, p), selectedFont(QFont())
{
	setSelectedFont(iF);
	QObject::connect(this, SIGNAL(clicked()), this, SLOT(doClicked()));
}

FontPickerButton::FontPickerButton(const QIcon &i, const QString &t, QWidget *p,
                                   const QFont &iF)
        : QPushButton(i, t, p), selectedFont(QFont())
{
	setSelectedFont(iF);
	QObject::connect(this, SIGNAL(clicked()), this, SLOT(doClicked()));
}

const QFont &FontPickerButton::getSelectedFont() const
{
	return selectedFont;
}

void FontPickerButton::setSelectedFont(const QFont &f)
{
	QFont display = selectedFont = f;

	if(display.pointSize() > -1)
		display.setPointSize(qMin(display.pointSize(), 24));
	else
		display.setPixelSize(qMin(display.pixelSize(), 50));

	setFont(display);
}

void FontPickerButton::setFont(const QFont &f)
{
	QPushButton::setFont(f);
}

void FontPickerButton::doClicked()
{
	bool ok;
	setSelectedFont(QFontDialog::getFont(&ok, getSelectedFont(), this,
	                                     tr("Select a Font"), nullptr));

	if(ok)
		Q_EMIT selectedFontChanged(getSelectedFont());
}
}
