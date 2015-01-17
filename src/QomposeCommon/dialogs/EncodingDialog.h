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
	EncodingDialog(QWidget * = nullptr, Qt::WindowFlags = nullptr,
	               const QString & = "UTF-8",
	               const QString & =
	                       tr("Which character encoding should be used?"),
	               const QString & = tr("Select Character Encoding"));
	virtual ~EncodingDialog();

	static QString
	promptEncoding(QWidget * = nullptr, const QString & = "UTF-8",
	               const QString & =
	                       tr("Which character encoding should be used?"),
	               const QString & = tr("Select Character Encoding"));

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

	EncodingDialog(const EncodingDialog &);
	EncodingDialog &operator=(const EncodingDialog &);

private Q_SLOTS:
	void doSelectClicked();
	void doCancelClicked();
};
}

#endif
