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

#include "QomposeSettings.h"

#include <QSettings>
#include <QStringList>
#include <QByteArray>
#include <QString>
#include <QFont>
#include <QColor>

const QList< QPair<QString, QVariant> > QomposeSettings::defaults
	= (QList< QPair<QString, QVariant> >())
		<< QPair<QString, QVariant>( "show-status-bar",        true                  )
		<< QPair<QString, QVariant>( "recent-list-size",       static_cast<int>(10)  )
		<< QPair<QString, QVariant>( "recent-list",            QList<QVariant>()     )
		<< QPair<QString, QVariant>( "window-save-attributes", true                  )
		<< QPair<QString, QVariant>( "show-gutter",            true                  )
		<< QPair<QString, QVariant>( "editor-font",            QFont("Courier", 11)  )
		<< QPair<QString, QVariant>( "editor-tab-width",       static_cast<int>(8)   )
		<< QPair<QString, QVariant>( "editor-foreground",      QColor(255, 255, 255) )
		<< QPair<QString, QVariant>( "editor-background",      QColor(39, 40, 34)    )
		<< QPair<QString, QVariant>( "editor-current-line",    QColor(70, 72, 61)    )
		<< QPair<QString, QVariant>( "gutter-foreground",      QColor(Qt::white)     )
		<< QPair<QString, QVariant>( "gutter-background",      QColor(Qt::black)     )
		<< QPair<QString, QVariant>( "window-geometry",        QByteArray()          )
		<< QPair<QString, QVariant>( "window-state",           QByteArray()          );

QomposeSettings::QomposeSettings(QObject *p)
	: QObject(p)
{
	settings = new QSettings(QSettings::UserScope,
		"Qompose", "Qompose", this);
	
	initializeDefaults();
}

QomposeSettings::~QomposeSettings()
{
	delete settings;
}

int QomposeSettings::count() const
{
	return settings->allKeys().count();
}

void QomposeSettings::resetDefaults()
{
	for(int i = 0; i < defaults.count(); ++i)
		setSetting(defaults.at(i).first, defaults.at(i).second);
}

void QomposeSettings::setSetting(const QString &k, const QVariant &v)
{
	settings->setValue(k, v);
	emit settingChanged(k, v);
}

bool QomposeSettings::containsSetting(const QString &k) const
{
	return settings->contains(k);
}

QVariant QomposeSettings::getSetting(const QString &k) const
{
	return settings->value(k);
}

void QomposeSettings::initializeDefaults()
{
	for(int i = 0; i < defaults.count(); ++i)
	{
		if(!containsSetting(defaults.at(i).first))
		{
			setSetting(defaults.at(i).first,
				defaults.at(i).second);
		}
	}
}
