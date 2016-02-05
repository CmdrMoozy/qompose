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

#include "EllipsizedLabel.h"

#include <QBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QStyleOption>

#include <QDebug>

#include "QomposeCommon/util/FontMetrics.h"

namespace
{
qreal getMinimumEllipsizedWidth(const QFont &f, const QString &t)
{
	qompose::FontMetrics metrics(f);

	if(t.length() > 5)
	{
		QString mint("");
		mint.append(t.at(0));
		mint.append("...");
		mint.append(t.at(t.length() - 1));
		return metrics.getWidth(mint);
	}
	else
	{
		// If the length of the string is 5 or less, we won't bother
		// ellipsizing it, because it's the same length (or less) as
		// e.g. x...x.
		return metrics.getWidth(t);
	}
}
}

namespace qompose
{
EllipsizedLabel::EllipsizedLabel(const QString &t, Qt::Alignment a, QWidget *p,
                                 Qt::WindowFlags f)
        : QFrame(p, f),
          alignment(a),
          originalText(),
          lastWidth(-1),
          ellipsizedText()
{
	setText(t);
}

EllipsizedLabel::EllipsizedLabel(Qt::Alignment a, QWidget *p, Qt::WindowFlags f)
        : EllipsizedLabel(tr(""), a, p, f)
{
}

Qt::Alignment EllipsizedLabel::getAlignment() const
{
	return alignment;
}

void EllipsizedLabel::setAlignment(Qt::Alignment a)
{
	alignment = a;
	update();
}

void EllipsizedLabel::setText(const QString &t)
{
	originalText = t;

	lastWidth = -1;
	ellipsizedText.clear();

	int minw = static_cast<int>(getMinimumEllipsizedWidth(font(), t));
	minw += contentsMargins().left() + contentsMargins().right();
	minw += frameWidth() * 2;
	setMinimumWidth(minw);

	update();
}

QString EllipsizedLabel::text() const
{
	return originalText;
}

void EllipsizedLabel::paintEvent(QPaintEvent *)
{
	FontMetrics metrics(font());
	QRect r = contentsRect();

	int w = getTextWidth();
	if(w != lastWidth)
	{
		lastWidth = w;
		ellipsizedText = metrics.ellipsizedText(originalText, w);
	}

	QStyleOption opt;
	opt.initFrom(this);

	QPainter painter(this);
	drawFrame(&painter);

	style()->drawItemText(&painter, r, getAlignment(), opt.palette,
	                      isEnabled(), ellipsizedText, foregroundRole());
}

int EllipsizedLabel::getHMargin() const
{
	auto cm = contentsMargins();
	return cm.left() + cm.right() + (frameWidth() * 2);
}

int EllipsizedLabel::getTextWidth() const
{
	return width() - getHMargin();
}

int EllipsizedLabel::textWidthToWidth(int w) const
{
	return w + getHMargin();
}
}
