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

#include "GoToDialog.h"

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>
#include <QPushButton>
#include <QShowEvent>
#include <QMessageBox>

#include "QomposeCommon/Defines.h"

namespace qompose
{

/*!
 * This is our default constructor, which initializes a new instance of our
 * "go to" dialog.
 *
 * \param p The parent widget for our dialog.
 * \param f The window falgs to use for our dialog.
 */
GoToDialog::GoToDialog(QWidget *p, Qt::WindowFlags f)
	: QDialog(p, f), selectedLine(0), layout(nullptr),
		lineLabel(nullptr), lineTextEdit(nullptr),
		buttonsWidget(nullptr), buttonsLayout(nullptr),
		closeButton(nullptr), goToButton(nullptr)
{
	setWindowTitle(tr("Go To Line"));

	initializeGUI();
}

/*!
 * This is our default destructor, which cleans up & destroys our dialog.
 */
GoToDialog::~GoToDialog()
{
}

/*!
 * This function returns the currently selected line. This value is 0 by
 * default, and is only updated when our "Go To" button is clicked.
 *
 * \return Our dialog's currently selected line.
 */
int GoToDialog::getSelectedLine() const
{
	return selectedLine;
}

/*!
 * This function handles our dialog being shown by resetting our dialog's contents,
 * setting focus on the correct dialog elements, and by raising our dialog to the
 * front.
 *
 * \param e The event being handled.
 */
void GoToDialog::showEvent(QShowEvent *e)
{
	// Setup our line text edit.

	lineTextEdit->setFocus();

	int l = getSelectedLine();

	if(l > 0)
		lineTextEdit->setText(QString("%1").arg(getSelectedLine()));
	else
		lineTextEdit->setText(QString(""));

	lineTextEdit->selectAll();

	// Bring our dialog to the front.

	raise();

	// Let our parent class do its thing.

	QDialog::showEvent(e);
}

/*!
 * This function initializes our dialog's GUI by creating our various
 * widgets and adding them to our layout.
 */
void GoToDialog::initializeGUI()
{
	layout = new QGridLayout(this);

	// Create our line inputs.

	lineLabel = new QLabel(tr("Destination line number:"), this, nullptr);

	lineTextEdit = new QLineEdit(this);

	// Create our buttons widget.

	buttonsWidget = new QWidget(this, nullptr);
	buttonsLayout = new QGridLayout(buttonsWidget);

	closeButton = new QPushButton(tr("Clos&e"), buttonsWidget);

	goToButton = new QPushButton(tr("&Go To"), buttonsWidget);
	goToButton->setDefault(true);

	buttonsLayout->addWidget(closeButton, 0, 1, 1, 1, nullptr);
	buttonsLayout->addWidget(goToButton, 0, 2, 1, 1, nullptr);
	buttonsLayout->setColumnStretch(0, 1);
	buttonsWidget->setLayout(buttonsLayout);

	// Add our widgets to our dialog.

	layout->addWidget(lineLabel, 0, 0, 1, 1, nullptr);
	layout->addWidget(lineTextEdit, 0, 1, 1, 1, nullptr);
	layout->addWidget(buttonsWidget, 2, 0, 1, 2, nullptr);
	layout->setColumnStretch(1, 1);
	layout->setRowStretch(1, 1);
	setLayout(layout);

	// Connect our actions.

	QObject::connect( goToButton,  SIGNAL( clicked(bool) ), this, SLOT( doGoTo() ) );
	QObject::connect( closeButton, SIGNAL( clicked(bool) ), this, SLOT( close()  ) );
}

/*!
 * This function performs our "Go To" operation by updating our selected line,
 * and then emitting a signal letting our callers know that our dialog has been
 * accepted.
 *
 * If the currently selected line number is invalid (i.e., isn't an integer), then
 * we will show an error instead, and avoid closing the dialog.
 */
void GoToDialog::doGoTo()
{ /* SLOT */

	QString l  = lineTextEdit->text();

	bool ok = false;
	int lint = l.trimmed().toInt(&ok, 10);

	if(ok)
	{
		selectedLine = lint;
		Q_EMIT accepted();
		close();
	}
	else
	{
		QMessageBox::critical(this, tr("Invalid Line Number"),
			tr("The line number you entered was invalid."),
			QMessageBox::Ok, QMessageBox::Ok);

		lineTextEdit->setFocus();
		lineTextEdit->selectAll();
	}

}

}
