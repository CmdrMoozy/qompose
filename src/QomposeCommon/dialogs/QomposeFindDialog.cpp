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

#include "QomposeFindDialog.h"

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QGroupBox>
#include <QCheckBox>
#include <QWidget>
#include <QPushButton>
#include <QShowEvent>

#include "QomposeDefines.h"
#include "util/QomposeFindQuery.h"

/*!
 * This is our default constructor, which creates a new instance of our
 * find dialog.
 *
 * \param p The parent widget to use for this dialog.
 * \param f The window flags to use for this dialog.
 */
QomposeFindDialog::QomposeFindDialog(QWidget *p, Qt::WindowFlags f)
	: QDialog(p, f), query(new QomposeFindQuery(this)),
		layout(nullptr), findLabel(nullptr), findTextEdit(nullptr),
		optionsGroupBox(nullptr), optionsLayout(nullptr),
		wrapCheckBox(nullptr), wholeWordsCheckBox(nullptr),
		caseSensitiveCheckBox(nullptr), reverseCheckBox(nullptr),
		regexCheckBox(nullptr), buttonsWidget(nullptr),
		buttonsLayout(nullptr), findButton(nullptr),
		closeButton(nullptr)
{
	setWindowTitle(tr("Find"));

	initializeGUI();

	setMinimumWidth(400);
}

/*!
 * This is our default destructor, which cleans up & destroys our dialog.
 */
QomposeFindDialog::~QomposeFindDialog()
{
}

/*!
 * This function returns a pointer to the find query our dialog currently
 * has. This pointer is still good even after the dialog has been re-shown.
 *
 * \return The find query containing our dialog's selected data.
 */
const QomposeFindQuery *QomposeFindDialog::getQuery() const
{
	return query;
}

/*!
 * This function handles our dialog being shown by resetting its
 * contents using our currently selected find query, by setting
 * focus on the expression input box, and by raising our dialog to
 * the front.
 *
 * \param e The event being handled.
 */
void QomposeFindDialog::showEvent(QShowEvent *e)
{
	// Setup our line text edit.

	findTextEdit->setFocus();
	findTextEdit->setText(query->getExpression());
	findTextEdit->selectAll();

	wrapCheckBox->setCheckState(query->isWrapping() ?
		Qt::Checked : Qt::Unchecked);

	wholeWordsCheckBox->setCheckState(query->isWholeWords() ?
		Qt::Checked : Qt::Unchecked);

	caseSensitiveCheckBox->setCheckState(query->isCaseSensitive() ?
		Qt::Checked : Qt::Unchecked);

	reverseCheckBox->setCheckState(query->isReversed() ?
		Qt::Checked : Qt::Unchecked);

	regexCheckBox->setCheckState(query->isRegularExpression() ?
		Qt::Checked : Qt::Unchecked);

	// Bring our dialog to the front.

	raise();

	// Let our parent class do its thing.

	QDialog::showEvent(e);
}

/*!
 * This function initializes our GUI by creating the various widgets we contain,
 * and adding them to our layout.
 */
void QomposeFindDialog::initializeGUI()
{
	layout = new QGridLayout(this);

	// Create our find expression inputs.

	findLabel = new QLabel(tr("Find expression:"), this, nullptr);

	findTextEdit = new QLineEdit();

	// Create our options group box.

	optionsGroupBox = new QGroupBox(tr("Options"), this);
	optionsLayout = new QGridLayout(optionsGroupBox);

	wrapCheckBox = new QCheckBox(tr("Wrap around document?"), optionsGroupBox);
	wrapCheckBox->setCheckState(Qt::Checked);

	wholeWordsCheckBox = new QCheckBox(tr("Find whole words only?"), optionsGroupBox);

	caseSensitiveCheckBox = new QCheckBox(tr("Case sensitive?"), optionsGroupBox);

	reverseCheckBox = new QCheckBox(tr("Reverse search direction?"), optionsGroupBox);

	regexCheckBox = new QCheckBox(tr("Regular expression search?"), optionsGroupBox);

	optionsLayout->addWidget(wrapCheckBox, 0, 0, 1, 1, nullptr);
	optionsLayout->addWidget(wholeWordsCheckBox, 1, 0, 1, 1, nullptr);
	optionsLayout->addWidget(caseSensitiveCheckBox, 2, 0, 1, 1, nullptr);
	optionsLayout->addWidget(reverseCheckBox, 3, 0, 1, 1, nullptr);
	optionsLayout->addWidget(regexCheckBox, 4, 0, 1, 1, nullptr);
	optionsLayout->setRowStretch(5, 1);
	optionsGroupBox->setLayout(optionsLayout);

	// Create our buttons widget.

	buttonsWidget = new QWidget(this, nullptr);
	buttonsLayout = new QGridLayout(buttonsWidget);

	findButton = new QPushButton(tr("&Find"), buttonsWidget);
	findButton->setDefault(true);

	closeButton = new QPushButton(tr("Clos&e"), buttonsWidget);

	buttonsLayout->addWidget(findButton, 0, 0, 1, 1, nullptr);
	buttonsLayout->addWidget(closeButton, 1, 0, 1, 1, nullptr);
	buttonsLayout->setRowStretch(2, 1);
	buttonsWidget->setLayout(buttonsLayout);

	// Add our widgets to our dialog.

	layout->addWidget(findLabel, 0, 0, 1, 1, nullptr);
	layout->addWidget(findTextEdit, 0, 1, 1, 1, nullptr);
	layout->addWidget(optionsGroupBox, 1, 0, 1, 2, nullptr);
	layout->addWidget(buttonsWidget, 0, 2, 2, 1, nullptr);
	layout->setRowStretch(1, 1);
	layout->setColumnStretch(1, 1);
	setLayout(layout);

	// Connect our button actions.

	QObject::connect( findButton,  SIGNAL( clicked(bool) ), this, SLOT( doFind() ) );
	QObject::connect( closeButton, SIGNAL( clicked(bool) ), this, SLOT( close()  ) );
}

/*!
 * This function handles our "find" button being clicked by applying our dialog's
 * contents to our find query object, and by alerting our callers that our dialog
 * has been accepted.
 */
void QomposeFindDialog::doFind()
{ /* SLOT */

	// Set our query's properties according to our dialog state.

	query->setExpression(findTextEdit->text());
	query->setWrapping(wrapCheckBox->checkState() == Qt::Checked);
	query->setWholeWords(wholeWordsCheckBox->checkState() == Qt::Checked);
	query->setCaseSensitive(caseSensitiveCheckBox->checkState() == Qt::Checked);
	query->setReversed(reverseCheckBox->checkState() == Qt::Checked);
	query->setRegularExpression(regexCheckBox->checkState() == Qt::Checked);

	// Done!

	Q_EMIT accepted();

	close();

}
