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

#include "Buffer.h"

#include <QTextCodec>
#include <QFile>
#include <QTextStream>
#include <QTextCursor>
#include <QFileInfo>
#include <QByteArray>
#include <QString>
#include <QVariant>
#include <QPrinter>
#include <QTextBlock>
#include <QDir>

#include "QomposeCommon/Defines.h"
#include "QomposeCommon/util/DocumentWriter.h"
#include "QomposeCommon/util/Settings.h"

namespace qompose
{
Buffer::Buffer(Settings *s, QWidget *p)
        : Editor(p), settings(s), path(QString()), codec(QString())
{
	// Load our initial settings, and connect our settings object.

	setGutterVisible(settings->getSetting("show-gutter").toBool());
	setFont(settings->getSetting("editor-font").value<QFont>());
	setIndentationWidth(
	        settings->getSetting("editor-indentation-width").toInt());
	setIndentationMode(settings->getSetting("editor-indentation-mode")
	                           .value<QString>());
	setWrapGuideVisible(
	        settings->getSetting("editor-wrap-guide-visible").toBool());
	setWrapGuideColumnWidth(
	        settings->getSetting("editor-wrap-guide-width").toInt());
	setWrapGuideColor(settings->getSetting("editor-wrap-guide-color")
	                          .value<QColor>());
	setEditorForeground(
	        settings->getSetting("editor-foreground").value<QColor>());
	setEditorBackground(
	        settings->getSetting("editor-background").value<QColor>());
	setCurrentLineColor(
	        settings->getSetting("editor-current-line").value<QColor>());
	setGutterForeground(
	        settings->getSetting("gutter-foreground").value<QColor>());
	setGutterBackground(
	        settings->getSetting("gutter-background").value<QColor>());

	QObject::connect(
	        settings,
	        SIGNAL(settingChanged(const QString &, const QVariant &)), this,
	        SLOT(doSettingChanged(const QString &, const QVariant &)));

	// Connect other various signals.

	QObject::connect(this, SIGNAL(modificationChanged(bool)), this,
	                 SLOT(doModificationChanged(bool)));
}

bool Buffer::open(const FileDescriptor &f)
{
	path = f.fileName;
	codec = f.textCodec;

	Q_EMIT pathChanged(path);

	bool r = read();

	if(r)
	{
		QTextCursor curs = textCursor();
		curs.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
		setTextCursor(curs);
	}

	return r;
}

bool Buffer::revert()
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
		{
			curs.setPosition(cursPos, QTextCursor::MoveAnchor);
		}
		else
		{
			curs.setPosition(QTextCursor::End,
			                 QTextCursor::MoveAnchor);
		}

		setTextCursor(curs);
	}

	return r;
}

bool Buffer::save(const QString &p)
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
		Q_EMIT pathChanged(path);

		return write();
	}
}

QString Buffer::getTitle() const
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

QString Buffer::getPath() const
{
	return path;
}

QString Buffer::getDirectory() const
{
	QString p = getPath();

	if(p.isNull())
		return QString();

	QFileInfo info(p);

	return info.dir().absolutePath();
}

QString Buffer::getFile() const
{
	QString p = getPath();

	if(p.isNull())
		return QString();

	QFileInfo info(p);

	return info.fileName();
}

FileDescriptor Buffer::getFileDescriptor() const
{
	FileDescriptor d = {path, codec};

	return d;
}

bool Buffer::hasBeenSaved() const
{
	return (!path.isNull());
}

bool Buffer::isModified() const
{
	return document()->isModified();
}

void Buffer::setModified(bool m)
{
	document()->setModified(m);
	Q_EMIT modificationChanged(m);
}

void Buffer::print(QPrinter *p)
{
	document()->print(p);
}

bool Buffer::read(bool u)
{
	QTextCodec *c = QTextCodec::codecForName(codec.toStdString().c_str());

	if(c == nullptr)
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

bool Buffer::write()
{
	// Get the right text codec for our encoding.

	QTextCodec *c = QTextCodec::codecForName(codec.toStdString().c_str());

	if(c == nullptr)
		return false;

	// Try opening the file we're going to write.

	QFile file(path);

	if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
		return false;

	// Setup our document writer.

	DocumentWriter writer(&file);

	writer.setCodec(c);
	writer.setWhitespaceTrimmed(
	        settings->getSetting("save-strip-trailing-spaces").toBool());

	bool r = writer.write(document());

	file.close();

	if(r)
		setModified(false);

	return r;
}

void Buffer::doModificationChanged(bool QUNUSED(c))
{
	Q_EMIT titleChanged(getTitle());
}

void Buffer::doSettingChanged(const QString &k, const QVariant &v)
{
	if(k == "show-gutter")
		setGutterVisible(v.toBool());
	else if(k == "editor-font")
		setFont(v.value<QFont>());
	else if(k == "editor-indentation-width")
		setIndentationWidth(v.toInt());
	else if(k == "editor-indentation-mode")
		setIndentationMode(v.value<QString>());
	else if(k == "editor-wrap-guide-visible")
		setWrapGuideVisible(v.toBool());
	else if(k == "editor-wrap-guide-width")
		setWrapGuideColumnWidth(v.toInt());
	else if(k == "editor-wrap-guide-color")
		setWrapGuideColor(v.value<QColor>());
	else if(k == "editor-foreground")
		setEditorForeground(v.value<QColor>());
	else if(k == "editor-background")
		setEditorBackground(v.value<QColor>());
	else if(k == "editor-current-line")
		setCurrentLineColor(v.value<QColor>());
	else if(k == "gutter-foreground")
		setGutterForeground(v.value<QColor>());
	else if(k == "gutter-background")
		setGutterBackground(v.value<QColor>());
}
}
