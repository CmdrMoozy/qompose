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
namespace detail
{
class EllipsizedLabelImpl : public QLabel
{
public:
	EllipsizedLabelImpl(const QString &t, QWidget *p = nullptr,
	                    Qt::WindowFlags f = nullptr)
	        : QLabel(p, f)
	{
		setText(t);
	}

	EllipsizedLabelImpl(const EllipsizedLabelImpl &) = delete;
	virtual ~EllipsizedLabelImpl() = default;

	EllipsizedLabelImpl &operator=(const EllipsizedLabelImpl &) = delete;

	virtual void setText(const QString &t)
	{
		originalText = t;

		int minw = getMinimumEllipsizedWidth(font(), t);
		minw += contentsMargins().left() + contentsMargins().right();
		minw += frameWidth() * 2;
		setMinimumWidth(minw);
	}

	virtual QString text() const
	{
		return originalText;
	}

protected:
	virtual void paintEvent(QPaintEvent *)
	{
		FontMetrics metrics(font());
		QRect r = contentsRect();
		r.adjust(margin(), margin(), -margin(), -margin());

		int w = getTextWidth();
		if(w != lastWidth)
		{
			lastWidth = w;
			ellipsizedText =
			        metrics.ellipsizedText(originalText, w);
		}

		QStyleOption opt;
		opt.initFrom(this);

		QPainter painter(this);
		drawFrame(&painter);

		style()->drawItemText(&painter, r, alignment(), opt.palette,
		                      isEnabled(), ellipsizedText,
		                      foregroundRole());
	}

private:
	QString originalText;

	int lastWidth;
	QString ellipsizedText;

	int getHMargin() const
	{
		auto cm = contentsMargins();
		return cm.left() + cm.right() + (frameWidth() * 2);
	}

	int getTextWidth() const
	{
		return width() - getHMargin();
	}

	int textWidthToWidth(int w) const
	{
		return w + getHMargin();
	}
};
}

EllipsizedLabel::EllipsizedLabel(const QString &t, QWidget *p, Qt::Alignment a,
                                 Qt::WindowFlags f)
        : QWidget(p, f), layout(nullptr), label(nullptr)
{
	layout = new QBoxLayout(QBoxLayout::Direction::LeftToRight, this);

	label = new detail::EllipsizedLabelImpl(t, this);
	label->setAlignment(a);

	layout->addWidget(label, 1);
	setLayout(layout);
}

EllipsizedLabel::EllipsizedLabel(QWidget *p, Qt::Alignment a, Qt::WindowFlags f)
        : EllipsizedLabel(tr(""), p, a, f)
{
}

void EllipsizedLabel::setText(const QString &t)
{
	label->setText(t);
}

QString EllipsizedLabel::text() const
{
	return label->text();
}

void EllipsizedLabel::setFrameStyle(int s)
{
	label->setFrameStyle(s);
}

int EllipsizedLabel::frameStyle() const
{
	return label->frameStyle();
}
}
