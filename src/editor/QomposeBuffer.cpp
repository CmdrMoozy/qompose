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
#include <QTextDocumentWriter>
#include <QByteArray>

#include "QomposeDefines.h"

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
	path = f.fileName;
	codec = f.textCodec;
	
	emit pathChanged(path);
	
	bool r = read();
	
	if(r)
	{
		QTextCursor curs = textCursor();
		curs.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
		setTextCursor(curs);
	}
	
	return r;
}

bool QomposeBuffer::revert()
{
	if(!hasBeenSaved())
		return false;
	
	QTextCursor curs = textCursor();
	int cursPos = curs.position();
	
	bool r = read(true);
	
	if(r)
	{
		QTextCursor endCurs = textCursor();
		endCurs.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
		
		int endPos = endCurs.position();
		
		if(cursPos <= endPos)
			curs.setPosition(cursPos, QTextCursor::MoveAnchor);
		else
			curs.setPosition(QTextCursor::End, QTextCursor::MoveAnchor);
		
		setTextCursor(curs);
	}
	
	return r;
}

bool QomposeBuffer::save(const QString &p)
{
	if(p.isNull())
	{
		if(!hasBeenSaved())
			return false;
		
		return write();
	}
	else
	{
		if(path.isNull())
			codec = "UTF-8";
		
		path = p;
		emit pathChanged(path);
		
		return write();
	}
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

/*!
 * This function (re-)reads our buffer's contents from the disk, using our object's
 * current path and text codec attributes.
 *
 * \param u Whether or not "undo" operations should be supported.
 */
bool QomposeBuffer::read(bool u)
{
	QTextCodec *c = QTextCodec::codecForName(codec.toStdString().c_str());
	
	if(c == 0)
		return false;
	
	QFile file(path);
	
	if(!file.open(QIODevice::ReadOnly))
		return false;
	
	QTextStream reader(&file);
	reader.setCodec(c);
	
	if(u)
	{
		selectAll();
		insertPlainText(reader.readAll());
	}
	else
	{
		setPlainText(reader.readAll());
	}
	
	setModified(false);
	
	return true;
}

bool QomposeBuffer::write()
{
	QTextCodec *c = QTextCodec::codecForName(codec.toStdString().c_str());
	
	if(c == 0)
		return false;
	
	QFile file(path);
	
	if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
		return false;
	
	QByteArray format("plaintext");
	QTextDocumentWriter writer(&file, format);
	
	writer.setCodec(c);
	bool r = writer.write(document());
	
	file.close();
	
	if(r)
		setModified(false);
	
	return r;
}

void QomposeBuffer::doModificationChanged(bool QUNUSED(c))
{ /* SLOT */
	
	emit titleChanged(getTitle());
	
}