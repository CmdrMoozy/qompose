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

#ifndef INCLUDE_QOMPOSECOMMON_DIALOGS_FILE_DIALOG_H
#define INCLUDE_QOMPOSECOMMON_DIALOGS_FILE_DIALOG_H

#include <QFileDialog>
#include <QString>
#include <QList>

#include "QomposeCommon/Types.h"

namespace qompose
{
/*!
 * \brief This class extends QFileDialog with character encoding detection.
 */
class FileDialog : public QFileDialog
{
public:
	/*!
	 * This is our default constructor, which creates a new file dialog
	 * instance.
	 *
	 * \param p The parent widget for our dialog.
	 * \param f The window flags to use for our dialog.
	 */
	FileDialog(QWidget *p, Qt::WindowFlags f);

	FileDialog(const FileDialog &) = delete;

	virtual ~FileDialog();

	FileDialog &operator=(const FileDialog &) = delete;

	/*!
	 * This is a factory function, which creates a new "null" file
	 * descriptor and returns it. "Null" file descriptors are those with
	 * the value QString() for both properties.
	 *
	 * \return A newly-created null file descriptor.
	 */
	static FileDescriptor getNullDescriptor();

	/*!
	 * This function will create a file descriptor for the file at the
	 * given path. The file's text codec will be detected or, failing that,
	 * the user will be prompted to select the text codec to use.
	 *
	 * \param p The path to the file to create a descriptor for.
	 * \return A file descriptor for the given path.
	 */
	static FileDescriptor getPathDescriptor(const QString &p);

	/*!
	 * This is a static utility function which displays an instance of our
	 * dialog for the purposes of opening a single file.
	 *
	 * \param p The parent widget to use for our dialog.
	 * \param c The caption to use for the dialog prompt.
	 * \param d The default directory to use for the dialog.
	 * \param f The filter to use for the open dialog.
	 * \param sf The selected filter to use for the open dialog.
	 * \param o The set of extra options to use for the open dialog.
	 * \return A valid file descriptor on "accept," or a null descriptor.
	 */
	static FileDescriptor getOpenFileName(QWidget *p = nullptr,
	                                      const QString &c = QString(),
	                                      const QString &d = QString(),
	                                      const QString &f = QString(),
	                                      QString *sf = nullptr,
	                                      QFileDialog::Options o = nullptr);

	/*!
	 * This is a static utility function which displays an instance of our
	 * dialog for the purposes of opening one or more files.
	 *
	 * \param p The parent widget to use for our dialog.
	 * \param c The caption to use for the dialog prompt.
	 * \param d The default directory to use for the dialog.
	 * \param f The filter to use for the open dialog.
	 * \param sf The selected filter to use for the open dialog.
	 * \param o The set of extra options to use for the open dialog.
	 * \return A list of file descriptors on "accept," or an empty list.
	 */
	static QList<FileDescriptor>
	getOpenFileNames(QWidget *p = nullptr, const QString &c = QString(),
	                 const QString &d = QString(),
	                 const QString &f = QString(), QString *sf = nullptr,
	                 QFileDialog::Options o = nullptr);

private:
	/*!
	 * This function tests whether or not the given file seems "good to
	 * go." This includes whether or not it is a regular file, is readable,
	 * and is reasonably sized (files larger than 5 MiB are considered
	 * unusually large).
	 *
	 * \param f The path to the file to inspect.
	 * \param p The parent widget for any prompts we display.
	 * \return True if the file looks good, or false otherwise.
	 */
	static bool fileIsGood(const QString &f, QWidget *p = nullptr);

	/*!
	 * This is a utility function which prompts the user to select a
	 * character encoding, in the case where the proper encoding couldn't
	 * be autodetected for a file.
	 *
	 * \param f The path to the file to prompt about.
	 * \return The user-selected character encoding for the given file.
	 */
	static QString promptTextCodec(const QString &f);
};
}

#endif
