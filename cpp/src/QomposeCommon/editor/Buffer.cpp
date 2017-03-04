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

#include <QByteArray>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QPrinter>
#include <QString>
#include <QTextBlock>
#include <QTextCodec>
#include <QTextCursor>
#include <QTextStream>
#include <QVariant>

#include <bdrck/fs/Util.hpp>

#include "core/Types.hpp"
#include "core/config/Configuration.hpp"

#include "QomposeCommon/Defines.h"
#include "QomposeCommon/editor/pane/Pane.h"
#include "QomposeCommon/fs/DocumentWriter.h"
#include "QomposeCommon/gui/BufferWidget.h"

namespace qompose
{
namespace editor
{
Buffer::Buffer(Pane *pp, QWidget *p)
        : Editor(p),
          configWatcher(new qompose::util::ConfigurationWatcher(this)),
          parentPane(pp),
          path(QString()),
          codec("UTF-8")
{
	// Load our initial settings, and connect our settings object.

	auto const &config = qompose::core::config::instance().get();
	setGutterVisible(config.show_gutter());
	setFont(qompose::core::config::toQFont(config.editor_font()));
	setIndentationWidth(config.editor_indentation_width());
	setIndentationMode(qompose::core::indentationModeFromMessage(
	        config.editor_indentation_mode()));
	setWrapGuideVisible(config.editor_wrap_guide_visible());
	setWrapGuideColumnWidth(config.editor_wrap_guide_width());
	setWrapGuideColor(qompose::core::config::toQColor(
	        config.editor_wrap_guide_color()));
	setEditorForeground(
	        qompose::core::config::toQColor(config.editor_foreground()));
	setEditorBackground(
	        qompose::core::config::toQColor(config.editor_background()));
	setCurrentLineColor(
	        qompose::core::config::toQColor(config.editor_current_line()));
	setGutterForeground(
	        qompose::core::config::toQColor(config.gutter_foreground()));
	setGutterBackground(
	        qompose::core::config::toQColor(config.gutter_background()));

	QObject::connect(configWatcher,
	                 SIGNAL(configurationFieldChanged(std::string const &)),
	                 this, SLOT(doSettingChanged(std::string const &)));

	// Connect other various signals.

	QObject::connect(this, SIGNAL(modificationChanged(bool)), this,
	                 SLOT(doModificationChanged(bool)));
}

Pane *Buffer::getParentPane() const
{
	return parentPane;
}

bool Buffer::open(const FileDescriptor &f)
{
	if(!setPath(f.fileName))
		return false;
	codec = f.textCodec;
	Q_EMIT(encodingChanged(f.textCodec.toLatin1()));

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

void Buffer::save()
{
	if(hasBeenSaved())
		write();
	else
		saveAs();
}

void Buffer::saveAs()
{
	// Open a save dialog and, on accept, write to the new path.

	QString defaultPath =
	        getParentPane()->getParentContainer()->getDefaultDirectory();
	if(hasBeenSaved())
	{
		if(!defaultPath.endsWith(QDir::separator()))
			defaultPath.append(QDir::separator());
		defaultPath.append(getFile());
	}

	QString savePath = QFileDialog::getSaveFileName(this, tr("Save Buffer"),
	                                                defaultPath, QString(),
	                                                nullptr, nullptr);
	if(savePath.length() <= 0)
		return;

	bdrck::fs::createFile(savePath.toStdString());
	setPath(savePath);
	write();
}

bool Buffer::prepareToClose()
{
	if(!isModified())
		return true;

	QMessageBox::StandardButton b = QMessageBox::question(
	        this, tr("Qompose - Unsaved Changes"),
	        tr("Save changes to this buffer before closing?"),
	        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
	        QMessageBox::Yes);

	if(b == QMessageBox::Yes)
	{
		save();
		if(!isModified())
			return true;
	}
	else if(b == QMessageBox::No)
	{
		return true;
	}

	return false;
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

	return info.dir().canonicalPath();
}

QString Buffer::getFile() const
{
	QString p = getPath();

	if(p.isEmpty())
		return QString();

	QFileInfo info(p);

	return info.fileName();
}

FileDescriptor Buffer::getFileDescriptor() const
{
	FileDescriptor d = {getPath(), codec};
	return d;
}

bool Buffer::hasBeenSaved() const
{
	return (!getPath().isEmpty());
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

QByteArray Buffer::getEncoding() const
{
	return codec.toLatin1();
}

void Buffer::setEncoding(const QByteArray &e)
{
	if(hasBeenSaved() && isModified())
	{
		auto button = QMessageBox::question(
		        this, tr("Qompose - Unsaved Changes"),
		        tr("Really discard buffer changes to change encoding?"),
		        QMessageBox::Yes | QMessageBox::Cancel,
		        QMessageBox::Yes);

		if(button == QMessageBox::Cancel)
			return;
	}

	codec = QString::fromLatin1(e);
	if(hasBeenSaved())
		revert();
}

void Buffer::print(QPrinter *p)
{
	document()->print(p);
}

bool Buffer::setPath(const QString &p)
{
	QString canonicalPath =
	        QString::fromStdString(bdrck::fs::resolvePath(p.toStdString()));
	if(canonicalPath.isEmpty())
		return false;
	path = canonicalPath;
	Q_EMIT pathChanged(path);
	return true;
}

bool Buffer::read(bool u)
{
	QTextCodec *c = QTextCodec::codecForName(codec.toLatin1());

	if(c == nullptr)
		return false;

	QFile file(getPath());

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

	QFile file(getPath());

	if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
		return false;

	// Setup our document writer.

	DocumentWriter writer(&file);

	writer.setCodec(c);
	writer.setWhitespaceTrimmed(qompose::core::config::instance()
	                                    .get()
	                                    .save_strip_trailing_spaces());

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

void Buffer::doSettingChanged(std::string const &name)
{
	auto const &config = qompose::core::config::instance().get();
	if(name == "show_gutter")
	{
		setGutterVisible(config.show_gutter());
	}
	else if(name == "editor_font")
	{
		setFont(qompose::core::config::toQFont(config.editor_font()));
	}
	else if(name == "editor_indentation_width")
	{
		setIndentationWidth(config.editor_indentation_width());
	}
	else if(name == "editor_indentation_mode")
	{
		setIndentationMode(qompose::core::indentationModeFromMessage(
		        config.editor_indentation_mode()));
	}
	else if(name == "editor_wrap_guide_visible")
	{
		setWrapGuideVisible(config.editor_wrap_guide_visible());
	}
	else if(name == "editor_wrap_guide_width")
	{
		setWrapGuideColumnWidth(config.editor_wrap_guide_width());
	}
	else if(name == "editor_wrap_guide_color")
	{
		setWrapGuideColor(qompose::core::config::toQColor(
		        config.editor_wrap_guide_color()));
	}
	else if(name == "editor_foreground")
	{
		setEditorForeground(qompose::core::config::toQColor(
		        config.editor_foreground()));
	}
	else if(name == "editor_background")
	{
		setEditorBackground(qompose::core::config::toQColor(
		        config.editor_background()));
	}
	else if(name == "editor_current_line")
	{
		setCurrentLineColor(qompose::core::config::toQColor(
		        config.editor_current_line()));
	}
	else if(name == "gutter_foreground")
	{
		setGutterForeground(qompose::core::config::toQColor(
		        config.gutter_foreground()));
	}
	else if(name == "gutter_background")
	{
		setGutterBackground(qompose::core::config::toQColor(
		        config.gutter_background()));
	}
}
}
}
