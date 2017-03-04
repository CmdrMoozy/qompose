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

#ifndef INCLUDE_QOMPOSECOMMON_GUI_ELLIPSIZED_LABEL_H
#define INCLUDE_QOMPOSECOMMON_GUI_ELLIPSIZED_LABEL_H

#include <QFrame>

namespace qompose
{
class EllipsizedLabel : public QFrame
{
public:
	EllipsizedLabel(const QString &t, Qt::Alignment a = Qt::AlignRight,
	                QWidget *p = nullptr, Qt::WindowFlags f = nullptr);
	EllipsizedLabel(Qt::Alignment a = Qt::AlignRight, QWidget *p = nullptr,
	                Qt::WindowFlags f = nullptr);
	EllipsizedLabel(const EllipsizedLabel &) = delete;
	virtual ~EllipsizedLabel() = default;

	EllipsizedLabel &operator=(const EllipsizedLabel &) = delete;

	Qt::Alignment getAlignment() const;
	void setAlignment(Qt::Alignment a);

	void setText(const QString &t);
	QString text() const;

protected:
	virtual void paintEvent(QPaintEvent *e);

private:
	Qt::Alignment alignment;
	QString originalText;

	int lastWidth;
	QString ellipsizedText;

	int getHMargin() const;
	int getTextWidth() const;
	int textWidthToWidth(int w) const;
};
}

#endif
