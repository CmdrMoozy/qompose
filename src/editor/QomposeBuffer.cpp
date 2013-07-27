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

#include "QomposeBuffer.h"

#include <QTextCodec>
#include <QFile>
#include <QTextStream>
#include <QTextCursor>
#include <QFileInfo>

QomposeBuffer::QomposeBuffer(QWidget *p)
	: QomposeEditor(p), path(QString())
{
	QObject::connect( this, SIGNAL( modificationChanged(bool) ),
		this, SLOT( doModificationChanged(bool) ) );
}

QomposeBuffer::~QomposeBuffer()
{
}

bool QomposeBuffer::open(const QomposeFileDescriptor &f)
{
	QTextCodec *codec = QTextCodec::codecForName(f.textCodec.toStdString().c_str());
	
	if(codec == 0)
		return false;
	
	QFile file(f.fileName);
	
	if(!file.open(QIODevice::ReadOnly))
		return false;
	
	QTextStream reader(&file);
	reader.setCodec(codec);
	
	setPlainText(reader.readAll());
	
	QTextCursor curs = textCursor();
	curs.movePosition(QTextCursor::Start);
	setTextCursor(curs);
	
	path = f.fileName;
	
	setModified(false);
	emit pathChanged(path);
	
	return true;
}

QString QomposeBuffer::getTitle() const
{
	QString title;
	
	if(hasBeenSaved())
	{
		QFileInfo fi(getPath());
		title = fi.fileName();
	}
	else
	{
		title = tr("Untitled");
	}
	
	if(isModified())
		title.append(" *");
	
	return title;
}

QString QomposeBuffer::getPath() const
{
	return path;
}

/*!
 * This function returns whether or not this buffer has ever been saved, or if it
 * is a brand-new document (possibly with unsaved changes).
 */
bool QomposeBuffer::hasBeenSaved() const
{
	return (!path.isNull());
}

bool QomposeBuffer::isModified() const
{
	return document()->isModified();
}

void QomposeBuffer::setModified(bool m)
{
	document()->setModified(m);
	emit modificationChanged(m);
}

void QomposeBuffer::doModificationChanged(bool c)
{ /* SLOT */
	
	emit titleChanged(getTitle());
	
}
