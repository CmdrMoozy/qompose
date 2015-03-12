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

#ifndef INCLUDE_QOMPOSECOMMON_EDITOR_BUFFER_H
#define INCLUDE_QOMPOSECOMMON_EDITOR_BUFFER_H

#include "QomposeCommon/editor/Editor.h"

#include "QomposeCommon/Types.h"

class QPrinter;

namespace qompose
{
class Settings;

/*!
 * \brief This class provides high-level buffer functionality for editors.
 */
class Buffer : public Editor
{
	Q_OBJECT

public:
	/*!
	 * This is our default constructor, which creates a new instance of
	 * our buffer widget.
	 *
	 * \param s The settings instance to get our preferences from.
	 * \param p The parent widget for our buffer widget.
	 */
	Buffer(Settings *s, QWidget *p = NULL);

	Buffer(const Buffer &) = delete;
	virtual ~Buffer() = default;

	Buffer &operator=(const Buffer &) = delete;

	/*!
	 * This function attempts to open the file denoted by the given file
	 * descriptor, and using the associated character encoding.
	 *
	 * \param f The file descriptor we will attempt to open.
	 * \return True on success, or false otherwise.
	 */
	bool open(const FileDescriptor &f);

	/*!
	 * This function reverts any unsaved changes our buffer might have by
	 * re-reading the contents of our currently open file.
	 *
	 * \return True on success, or false otherwise.
	 */
	bool revert();

	/*!
	 * This function attempts to save the current contents of our buffer to
	 * a file. If the given path is QString(), then we will use our
	 * existing internal path instead of saving to a brand new file.
	 *
	 * \param p The path to save our contents to.
	 * \return True on success, or false otherwise.
	 */
	bool save(const QString &p = QString());

	/*!
	 * This function computes the "title" of our buffer, which is dependant
	 * on our currently open filename, as well as whether or not our buffer
	 * has ever been saved.
	 *
	 * \return Our buffer's human-readable title.
	 */
	QString getTitle() const;

	/*!
	 * This function returns the path to the file this buffer is currently
	 * representing, or QString() if we don't have a valid file.
	 *
	 * \return The path to this buffer's current file.
	 */
	QString getPath() const;

	/*!
	 * This function returns the path to the directory the file this buffer
	 * is currently representing resides in. If we don't have a valid path
	 * (e.g., this buffer has never been saved), then we return QString()
	 * instead.
	 *
	 * \return The path to this buffer's current file's parent directory.
	 */
	QString getDirectory() const;

	/*!
	 * This function returns the name of the file this buffer is
	 * representing, without any path information. If we don't have a valid
	 * path (e.g., this buffer has never been saved), then we return
	 * QString() instead.
	 *
	 * \return The name of this buffer's current file.
	 */
	QString getFile() const;

	/*!
	 * This function returns this buffer's current file, along with the
	 * file's encoding, using a standard FileDescriptor.
	 *
	 * \return The buffer's current file, as a FileDescriptor.
	 */
	FileDescriptor getFileDescriptor() const;

	/*!
	 * This function returns whether or not this buffer has ever been
	 * saved, or if it is a brand-new document (possibly with unsaved
	 * changes).
	 */
	bool hasBeenSaved() const;

	/*!
	 * This function returns whether or not our contents have been altered
	 * since our buffer's last save or load action.
	 *
	 * \return True if our buffer has been modified, or false otherwise.
	 */
	bool isModified() const;

	/*!
	 * This function sets our buffer's modified status to the given value,
	 * and emits a modificationChanged() signal.
	 *
	 * \param m The new modified status for our buffer.
	 */
	void setModified(bool m);

public Q_SLOTS:
	/*!
	 * This slot prints our buffer's contents to the given printer object.
	 *
	 * \param p The printer to print our contents to.
	 */
	void print(QPrinter *p);

private:
	Settings *settings;

	QString path;
	QString codec;

	/*!
	 * This function (re-)reads our buffer's contents from the disk, using
	 * our object's current path and text codec attributes.
	 *
	 * \param u Whether or not "undo" operations should be supported.
	 * \return True on success, or false otherwise.
	 */
	bool read(bool u = false);

	/*!
	 * This is a utility function which writes the contents of our buffer
	 * to our buffer's current file path.
	 *
	 * \return True on success, or false otherwise.
	 */
	bool write();

private Q_SLOTS:
	/*!
	 * This function handles our modification state being changed by
	 * emitting a titleChanged() signal, letting our callers know that we
	 * have a new title.
	 *
	 * \param c The new modification status - this is ignored.
	 */
	void doModificationChanged(bool c);

	/*!
	 * This function handles a setting being changed by, if it's a setting
	 * this widget cares about, updating our object's properties
	 * accordingly.
	 *
	 * \param k The setting key whose value was changed.
	 * \param v The new value for this particular setting.
	 */
	void doSettingChanged(const QString &k, const QVariant &v);

Q_SIGNALS:
	void titleChanged(const QString &);
	void pathChanged(const QString &);
};
}

#endif