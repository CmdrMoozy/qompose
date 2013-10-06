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

#ifndef INCLUDE_QOMPOSE_FONT_PICKER_BUTTON_H
#define INCLUDE_QOMPOSE_FONT_PICKER_BUTTON_H

#include <QPushButton>
#include <QFont>

class QString;
class QIcon;

/*!
 * \brief This class implements a QPushButton specifically for selecting a font.
 *
 * When clicked, this button automatically prompts the user for a new font. To demonstrate
 * the font selected, we set our widget's font to the one the user chose. Note that, to preserve
 * layouts, we do limit the font size we will display -- but any size of font can be selected,
 * and will be returned by getSelectedFont() appropriately.
 */
class QomposeFontPickerButton : public QPushButton
{
	
	Q_OBJECT
	
	public:
		QomposeFontPickerButton(QWidget *p = 0, const QFont &iF = QFont());
		QomposeFontPickerButton(const QString &t, QWidget *p = 0,
			const QFont &iF = QFont());
		QomposeFontPickerButton(const QIcon &i, const QString &t,
			QWidget *p = 0, const QFont &iF = QFont());
		virtual ~QomposeFontPickerButton();
	
		const QFont &getSelectedFont() const;
		void setSelectedFont(const QFont &f);
		
	private:
		QFont selectedFont;
	
		virtual void setFont(const QFont &f);
		
	private Q_SLOTS:
		void doClicked();
		
	Q_SIGNALS:
		void selectedFontChanged(const QFont &);
		
};

#endif
