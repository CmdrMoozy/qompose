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

#ifndef INCLUDE_QOMPOSE_RECENT_MENU_H
#define INCLUDE_QOMPOSE_RECENT_MENU_H

#include <QObject>

#include <QQueue>
#include <QString>
#include <QList>

class QMenu;
class QAction;

class QomposeSettings;

/*!
 * \brief This object implements a "recently opened files" menu.
 */
class QomposeRecentMenu : public QObject
{
	Q_OBJECT
	
	public:
		QomposeRecentMenu(QomposeSettings *s, QObject *p = 0);
		virtual ~QomposeRecentMenu();
		
		int getCapacity() const;
		QMenu *getMenu() const;
		
		void addPath(const QString &p);
		
		void saveContents();
		
	private:
		QomposeSettings *settings;
		int capacity;
		
		QMenu *menu;
		QList<QAction *> menuActions;
		
		QQueue<QString> recentList;
		
		void updateActionsListSize();
		void setCapacity(int c);
		void renderListContents();
		void setListContents(const QStringList &l);
		
	private Q_SLOTS:
		void doActionClicked();
		void doSettingChanged(const QString &k, const QVariant &v);
		
	Q_SIGNALS:
		void recentClicked(const QString &);
};

#endif
