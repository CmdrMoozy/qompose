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

#ifndef INCLUDE_QOMPOSE_COLOR_PICKER_BUTTON_H
#define INCLUDE_QOMPOSE_COLOR_PICKER_BUTTON_H

#include <QPushButton>
#include <QColor>

class QString;
class QIcon;
class QPaintEvent;

/*!
 * \brief This class implements a QPushButton specifically for selecting a color.
 *
 * When clicked, this button automatically prompts the user for a new color. To demonstrate
 * the color selected, our widget displays a solid rectangle filled with the color instead of
 * text or an icon or whatever else.
 */
class QomposeColorPickerButton : public QPushButton
{
	Q_OBJECT
	
	public:
		QomposeColorPickerButton(QWidget *p = 0, const QColor &iC = Qt::black);
		QomposeColorPickerButton(const QString &t, QWidget *p = 0,
			const QColor &iC = Qt::black);
		QomposeColorPickerButton(const QIcon &i, const QString &t,
			QWidget *p = 0, const QColor &iC = Qt::black);
		virtual ~QomposeColorPickerButton();
	
		const QColor &getSelectedColor() const;
		void setSelectedColor(const QColor &c);
	
	protected:
		virtual void paintEvent(QPaintEvent *e);
	
	private:
		QColor selectedColor;
	
		virtual void setIcon(const QIcon &i);
		virtual void setText(const QString &t);
	
	private slots:
		void doClicked();
	
	signals:
		void selectedColorChanged(const QColor &);
};

#endif

