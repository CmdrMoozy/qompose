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

#include "QomposeEncodingDialog.h"

#include <QGridLayout>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QList>
#include <QByteArray>
#include <QTextCodec>
#include <QListWidgetItem>

#include "QomposeDefines.h"

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
QomposeEncodingDialog::QomposeEncodingDialog(QWidget *p, Qt::WindowFlags f,
	const QString &d, const QString &m, const QString &t)
	: QDialog(p, f), encoding(QString()), layout(nullptr),
		messageLabel(nullptr), encodingList(nullptr),
		buttonsWidget(nullptr), buttonsLayout(nullptr),
		selectButton(nullptr), cancelButton(nullptr)
{
	layout = new QGridLayout(this);

	messageLabel = new QLabel(m, this, nullptr);

	encodingList = new QListWidget(this);

	QList<QByteArray> codecs = QTextCodec::availableCodecs();

	for(int i = 0; i < codecs.size(); ++i)
		encodingList->addItem(QString(codecs.at(i)));

	QList<QListWidgetItem *> defaultEncoding =
		encodingList->findItems(d, Qt::MatchExactly);

	if(defaultEncoding.size() == 1)
		encodingList->setCurrentItem(defaultEncoding.first());

	buttonsWidget = new QWidget(this, nullptr);
	buttonsLayout = new QGridLayout(buttonsWidget);

	selectButton = new QPushButton(tr("&Select"), buttonsWidget);

	cancelButton = new QPushButton(tr("&Cancel"), buttonsWidget);

	buttonsLayout->addWidget(selectButton, 0, 1, 1, 1, nullptr);
	buttonsLayout->addWidget(cancelButton, 0, 2, 1, 1, nullptr);
	buttonsLayout->setColumnStretch(0, 1);
	buttonsWidget->setLayout(buttonsLayout);

	layout->addWidget(messageLabel, 0, 0, 1, 1, nullptr);
	layout->addWidget(encodingList, 1, 0, 1, 1, nullptr);
	layout->addWidget(buttonsWidget, 2, 0, 1, 1, nullptr);
	setLayout(layout);

	QObject::connect(selectButton, SIGNAL(clicked(bool)),
		this, SLOT(doSelectClicked()));
	QObject::connect(cancelButton, SIGNAL(clicked(bool)),
		this, SLOT(doCancelClicked()));

	setModal(true);
	setWindowTitle(t);
}

/*!
 * This is our default destructor, which cleans up & destroys our dialog.
 */
QomposeEncodingDialog::~QomposeEncodingDialog()
{
}

/*!
 * This function creates a new encoding dialog, allowing the user to select a
 * character encoding to use. This dialog will show the given message, as well
 * as a combo box with all possible character encodings in it, and it has two
 * buttons - "Select" and "Cancel".
 *
 * The string returned by this function will be valid for QTextCodec's
 * codecForName function.
 *
 * If no character encoding is selected (i.e., the "Cancel" button is clicked,
 * or the dialog is closed), then we return a null QString instead.
 *
 * \param p The parent widget for the dialog.
 * \param d The default character encoding.
 * \param m The message to display in the dialog.
 * \param t The title of the dialog.
 * \return The selected character encoding.
 */
QString QomposeEncodingDialog::promptEncoding(QWidget *p, const QString &d,
	const QString &m, const QString &t)
{

	QomposeEncodingDialog dialog(p, nullptr, d, m, t);
	dialog.exec();
	return dialog.getSelectedEncoding();

}

/*!
 * This function returns the encoding selected by our user. If the dialog was
 * "accepted," then this returns the selected encoding. If the dialog was
 * canceled, or no valid encoding was selected, then this returns QString()
 * instead.
 *
 * \return The currently selected character encoding.
 */
QString QomposeEncodingDialog::getSelectedEncoding() const
{
	return encoding;
}

/*!
 * This function handles our select button being clicked by updating our
 * dialog's selected encoding, and then closes our dialog.
 */
void QomposeEncodingDialog::doSelectClicked()
{ /* SLOT */

	QList<QListWidgetItem *> selected = encodingList->selectedItems();

	if(selected.size() == 1)
		encoding = selected.first()->text();
	else
		encoding = QString();

	done(0);

}

/*!
 * This function handles our cancel button being clicked by clearing our
 * dialog's selected encoding, and then closes our dialog.
 */
void QomposeEncodingDialog::doCancelClicked()
{ /* SLOT */

	encoding = QString();
	done(0);

}
