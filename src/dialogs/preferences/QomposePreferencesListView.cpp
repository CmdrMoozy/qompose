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

#include "QomposePreferencesListView.h"

#include <QFont>
#include <QAbstractItemModel>
#include <QString>
#include <QFontMetrics>

#include "dialogs/preferences/QomposePreferencesListModel.h"
#include "dialogs/preferences/widgets/QomposePreferencesWidget.h"

QomposePreferencesListView::QomposePreferencesListView(QWidget *p)
	: QListView(p)
{
	setIconSize(QSize(30, 30));
	setSpacing(5);
	
	QFont f = font();
	f.setPointSize(10);
	setFont(f);
}

QomposePreferencesListView::~QomposePreferencesListView()
{
}

void QomposePreferencesListView::setModel(QomposePreferencesListModel *m)
{
	QAbstractItemModel *aim = dynamic_cast<QAbstractItemModel *>(m);
	
	setModel(aim);
}

QSize QomposePreferencesListView::sizeHint() const
{
	// Get our view's model (if applicable).
	
	QSize s;
	QomposePreferencesListModel *m = dynamic_cast<QomposePreferencesListModel *>(model());
	
	if(m == NULL)
		return QListView::sizeHint();
	
	// Compute the height and width we need to display all of our items.
	
	s.setHeight(50 * m->rowCount());
	
	QFontMetrics fm(font());
	int maxWidth = 0;
	
	for(int i = 0; i < m->rowCount(); ++i)
	{
		const QomposePreferencesWidget *w = m->widgetAt(i);
		
		if(w == NULL)
			continue;
		
		QString text = w->getPreferencesTitle();
		
		maxWidth = qMax(maxWidth, fm.width(text));
	}
	
	s.setWidth( maxWidth + 75 );
	
	// Use some default minimums.
	
	s.setWidth( qMax(s.width(), 100) );
	s.setHeight( qMax(s.height(), 200) );
	
	// Done!
	
	return s;
}

void QomposePreferencesListView::setModel(QAbstractItemModel *m)
{
	QListView::setModel(m);
}
