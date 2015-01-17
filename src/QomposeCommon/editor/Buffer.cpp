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
/*!
 * This is our default constructor, which creates a new instance of
 * our buffer widget.
 *
 * \param s The settings instance to get our preferences from.
 * \param p The parent widget for our buffer widget.
 */
Buffer::Buffer(Settings *s, QWidget *p)
        : Editor(p), settings(s), path(QString()), codec(QString())
{
	// Load our initial settings, and connect our settings object.

	setGutterVisible(settings->getSetting("show-gutter").toBool());
	setFont(settings->getSetting("editor-font").value<QFont>());
	setTabWidthSpaces(
	        settings->getSetting("editor-indentation-width").toInt());
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

/*!
 * This is our default destructor, which cleans up & destroys our widget.
 */
Buffer::~Buffer()
{
}

/*!
 * This function attempts to open the file denoted by the given file
 * descriptor, and using the associated character encoding.
 *
 * \param f The file descriptor we will attempt to open.
 * \return True on success, or false otherwise.
 */
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

/*!
 * This function reverts any unsaved changes our buffer might have by
 * re-reading the contents of our currently open file.
 *
 * \return True on success, or false otherwise.
 */
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

/*!
 * This function attempts to save the current contents of our buffer to a
 * file. If the given path is QString(), then we will use our existing
 * internal path instead of saving to a brand new file.
 *
 * \param p The path to save our contents to.
 * \return True on success, or false otherwise.
 */
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

/*!
 * This function computes the "title" of our buffer, which is dependant on our
 * currently open filename, as well as whether or not our buffer has ever been
 * saved.
 *
 * \return Our buffer's human-readable title.
 */
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

/*!
 * This function returns the path to the file this buffer is currently
 * representing, or QString() if we don't have a valid file.
 *
 * \return The path to this buffer's current file.
 */
QString Buffer::getPath() const
{
	return path;
}

/*!
 * This function returns the path to the directory the file this buffer is
 * currently representing resides in. If we don't have a valid path (e.g., this
 * buffer has never been saved), then we return QString() instead.
 *
 * \return The path to this buffer's current file's parent directory.
 */
QString Buffer::getDirectory() const
{
	QString p = getPath();

	if(p.isNull())
		return QString();

	QFileInfo info(p);

	return info.dir().absolutePath();
}

/*!
 * This function returns the name of the file this buffer is representing,
 * without any path information. If we don't have a valid path (e.g., this
 * buffer has never been saved), then we return QString() instead.
 *
 * \return The name of this buffer's current file.
 */
QString Buffer::getFile() const
{
	QString p = getPath();

	if(p.isNull())
		return QString();

	QFileInfo info(p);

	return info.fileName();
}

/*!
 * This function returns this buffer's current file, along with the file's
 * encoding, using a standard FileDescriptor.
 *
 * \return The buffer's current file, as a FileDescriptor.
 */
FileDescriptor Buffer::getFileDescriptor() const
{
	FileDescriptor d = {path, codec};

	return d;
}

/*!
 * This function returns whether or not this buffer has ever been saved, or if
 * it is a brand-new document (possibly with unsaved changes).
 */
bool Buffer::hasBeenSaved() const
{
	return (!path.isNull());
}

/*!
 * This function returns whether or not our contents have been altered since
 * our buffer's last save or load action.
 *
 * \return True if our buffer has been modified, or false otherwise.
 */
bool Buffer::isModified() const
{
	return document()->isModified();
}

/*!
 * This function sets our buffer's modified status to the given value, and
 * emits a modificationChanged() signal.
 *
 * \param m The new modified status for our buffer.
 */
void Buffer::setModified(bool m)
{
	document()->setModified(m);
	Q_EMIT modificationChanged(m);
}

/*!
 * This slot prints our buffer's contents to the given printer object.
 *
 * \param p The printer to print our contents to.
 */
void Buffer::print(QPrinter *p)
{ /* SLOT */
	document()->print(p);
}

/*!
 * This function (re-)reads our buffer's contents from the disk, using our
 * object's current path and text codec attributes.
 *
 * \param u Whether or not "undo" operations should be supported.
 * \return True on success, or false otherwise.
 */
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

/*!
 * This is a utility function which writes the contents of our buffer to our
 * buffer's current file path.
 *
 * \return True on success, or false otherwise.
 */
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

/*!
 * This function handles our modification state being changed by emitting a
 * titleChanged() signal, letting our callers know that we have a new title.
 *
 * \param c The new modification status - this is ignored.
 */
void Buffer::doModificationChanged(bool QUNUSED(c))
{ /* SLOT */

	Q_EMIT titleChanged(getTitle());
}

/*!
 * This function handles a setting being changed by, if it's a setting this
 * widget cares about, updating our object's properties accordingly.
 *
 * \param k The setting key whose value was changed.
 * \param v The new value for this particular setting.
 */
void Buffer::doSettingChanged(const QString &k, const QVariant &v)
{ /* SLOT */

	if(k == "show-gutter")
		setGutterVisible(v.toBool());
	else if(k == "editor-font")
		setFont(v.value<QFont>());
	else if(k == "editor-indentation-width")
		setTabWidthSpaces(v.toInt());
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
