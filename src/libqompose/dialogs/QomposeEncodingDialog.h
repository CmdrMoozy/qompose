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

#ifndef INCLUDE_QOMPOSE_ENCODING_DIALOG_H
#define INCLUDE_QOMPOSE_ENCODING_DIALOG_H

#include <QDialog>
#include <QString>

class QGridLayout;
class QLabel;
class QListWidget;
class QPushButton;

/*!
 * \brief This class implements a simple text selection encoding dialog.
 */
class QomposeEncodingDialog : public QDialog
{
	Q_OBJECT

	public:
		QomposeEncodingDialog(QWidget *p = 0, Qt::WindowFlags f = 0,
			const QString &d = "UTF-8",
			const QString &m = tr("Which character encoding should be used?"),
			const QString &t = tr("Select Character Encoding"));
		virtual ~QomposeEncodingDialog();

		static QString promptEncoding(QWidget *p = 0, const QString &d = "UTF-8",
			const QString &m = tr("Which character encoding should be used?"),
			const QString &t = tr("Select Character Encoding"));

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
		void doSelectClicked();
		void doCancelClicked();
};

#endif