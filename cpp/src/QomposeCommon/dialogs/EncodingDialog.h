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

#ifndef INCLUDE_QOMPOSECOMMON_DIALOGS_ENCODING_DIALOG_H
#define INCLUDE_QOMPOSECOMMON_DIALOGS_ENCODING_DIALOG_H

#include <QDialog>
#include <QString>

class QGridLayout;
class QLabel;
class QListWidget;
class QPushButton;

namespace qompose
{
/*!
 * \brief This class implements a simple text selection encoding dialog.
 */
class EncodingDialog : public QDialog
{
	Q_OBJECT

public:
	/*!
	 * This is our default constructor, which creates a new instance of our
	 * character encoding dialog.
	 *
	 * \param p The parent widget for this dialog.
	 * \param f The window flags to use for this dialog.
	 * \param d The encoding to select by default in our list of encodings.
	 * \param m The message to prompt the user with.
	 * \param t The window title for the dialog.
	 */
	EncodingDialog(QWidget *p = nullptr, Qt::WindowFlags f = nullptr,
	               const QString &d = "UTF-8",
	               const QString &m =
	                       tr("Which character encoding should be used?"),
	               const QString &t = tr("Select Character Encoding"));

	EncodingDialog(const EncodingDialog &) = delete;

	virtual ~EncodingDialog() = default;

	EncodingDialog &operator=(const EncodingDialog &) = delete;

	/*!
	 * This function creates a new encoding dialog, allowing the user to
	 * select a character encoding to use. This dialog will show the given
	 * message, as well as a combo box with all possible character
	 * encodings in it, and it has two buttons - "Select" and "Cancel".
	 *
	 * The string returned by this function will be valid for QTextCodec's
	 * codecForName function.
	 *
	 * If no character encoding is selected (i.e., the "Cancel" button is
	 * clicked, or the dialog is closed), then we return a null QString
	 * instead.
	 *
	 * \param p The parent widget for the dialog.
	 * \param d The default character encoding.
	 * \param m The message to display in the dialog.
	 * \param t The title of the dialog.
	 * \return The selected character encoding.
	 */
	static QString
	promptEncoding(QWidget *p = nullptr, const QString &d = "UTF-8",
	               const QString &m =
	                       tr("Which character encoding should be used?"),
	               const QString &t = tr("Select Character Encoding"));

	/*!
	 * This function returns the encoding selected by our user. If the
	 * dialog was "accepted," then this returns the selected encoding. If
	 * the dialog was canceled, or no valid encoding was selected, then
	 * this returns QString() instead.
	 *
	 * \return The currently selected character encoding.
	 */
	QString getSelectedEncoding() const;

private:
	QString encoding;

	QGridLayout *layout;
	QLabel *messageLabel;
	QListWidget *encodingList;

	QWidget *buttonsWidget;
	QGridLayout *buttonsLayout;
	QPushButton *selectButton;
	QPushButton *cancelButton;

private Q_SLOTS:
	/*!
	 * This function handles our select button being clicked by updating
	 * our dialog's selected encoding, and then closes our dialog.
	 */
	void doSelectClicked();

	/*!
	 * This function handles our cancel button being clicked by clearing
	 * our dialog's selected encoding, and then closes our dialog.
	 */
	void doCancelClicked();
};
}

#endif
