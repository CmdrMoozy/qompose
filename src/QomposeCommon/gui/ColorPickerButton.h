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

#ifndef INCLUDE_QOMPOSECOMMON_GUI_COLOR_PICKER_BUTTON_H
#define INCLUDE_QOMPOSECOMMON_GUI_COLOR_PICKER_BUTTON_H

#include <QColor>
#include <QPushButton>

class QString;
class QIcon;
class QPaintEvent;

namespace qompose
{
/*!
 * \brief This class implements a QPushButton for selecting a color.
 *
 * When clicked, this button automatically prompts the user for a new color.
 * To demonstrate the color selected, our widget displays a solid rectangle
 * filled with the color instead of text or an icon or whatever else.
 */
class ColorPickerButton : public QPushButton
{
	Q_OBJECT

public:
	/*!
	 * This is one of our constructors, which intializes a new color picker
	 * object with the given parameters.
	 *
	 * \param p Our parent widget.
	 * \param iC Our initial color.
	 */
	ColorPickerButton(QWidget *p = nullptr, const QColor &iC = Qt::black);

	/*!
	 * This is one of our constructors, which intializes a new color picker
	 * object with the given parameters.
	 *
	 * \param t Our button's text (IGNORED).
	 * \param p Our parent widget.
	 * \param iC Our initial color.
	 */
	ColorPickerButton(const QString &t, QWidget *p = nullptr,
	                  const QColor &iC = Qt::black);

	/*!
	 * This is one of our constructors, which intializes a new color picker
	 * object with the given parameters.
	 *
	 * \param i Our button's icon (IGNORED).
	 * \param t Our button's text (IGNORED).
	 * \param p Our parent widget.
	 * \param iC Our initial color.
	 */
	ColorPickerButton(const QIcon &i, const QString &t,
	                  QWidget *p = nullptr, const QColor &iC = Qt::black);

	ColorPickerButton(const ColorPickerButton &) = delete;
	virtual ~ColorPickerButton() = default;

	ColorPickerButton &operator=(const ColorPickerButton &) = delete;

	/*!
	 * This function simply returns the color we are currently
	 * representing. This is also the color that is displayed by our
	 * button.
	 *
	 * \return Our currently selected color.
	 */
	const QColor &getSelectedColor() const;

	/*!
	 * This function allows the currently selected color to be set. This
	 * will be the color that our button displays.
	 *
	 * Note that this function does NOT emit a selectedColorChanged()
	 * signal - that is reserved for the color being changed by the USER
	 * when they click us.
	 *
	 * \param c The new color to represent.
	 */
	void setSelectedColor(const QColor &c);

protected:
	/*!
	 * We override our parent class's paint event so we can draw the color
	 * we are representing on our button.
	 *
	 * \param e The event we are handling.
	 */
	virtual void paintEvent(QPaintEvent *e);

private:
	QColor selectedColor;

	/*!
	 * We override our parent class's setIcon() function, since this
	 * particular button does not, by definition, display icons. Note that
	 * this function still performs its function correctly, but is private.
	 *
	 * \param i The new icon to display.
	 */
	virtual void setIcon(const QIcon &i);

	/*!
	 * We override our parent class's setText() function, since this
	 * particular button does not, by definition, display text. Note that
	 * this function still performs its function correctly, but is private.
	 *
	 * \param t The new text to display.
	 */
	virtual void setText(const QString &t);

private Q_SLOTS:
	/*!
	 * When we are clicked, we want to automatically pop up a QColorDialog
	 * to get a new color selection from the user. If the user selects a
	 * new color (i.e., doesn't click "Cancel"), then we emit a
	 * selectedColorChanged() signal.
	 */
	void doClicked();

Q_SIGNALS:
	void selectedColorChanged(const QColor &);
};
}

#endif
