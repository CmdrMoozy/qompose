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

#include "QomposeReplaceDialog.h"

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QGroupBox>
#include <QCheckBox>
#include <QWidget>
#include <QPushButton>
#include <QShowEvent>

#include "util/QomposeReplaceQuery.h"

/*!
 * This is our default constructor, which creates a new instance of our
 * replace dialog.
 *
 * \param p The parent widget to use for this dialog.
 * \param f The window flags to use for this dialog.
 */
QomposeReplaceDialog::QomposeReplaceDialog(QWidget *p, Qt::WindowFlags f)
	: QDialog(p, f)
{
	setWindowTitle(tr("Replace"));

	query = new QomposeReplaceQuery(this);

	initializeGUI();

	setMinimumWidth(450);
}

/*!
 * This is our default destructor, which cleans up & destroys our dialog.
 */
QomposeReplaceDialog::~QomposeReplaceDialog()
{
}

/*!
 * This function returns a pointer to the replace query our dialog currently
 * has. This pointer is still good even after the dialog has been re-shown.
 *
 * \return The replace query containing our dialog's selected data.
 */
const QomposeReplaceQuery *QomposeReplaceDialog::getQuery() const
{
	return query;
}

/*!
 * This function handles our dialog being shown by resetting its
 * contents using our currently selected replace query, by setting
 * focus on the expression input box, and by raising our dialog to
 * the front.
 *
 * \param e The event being handled.
 */
void QomposeReplaceDialog::showEvent(QShowEvent *e)
{
	// Setup our line text edit.

	findTextEdit->setFocus();
	findTextEdit->setText(query->getExpression());
	findTextEdit->selectAll();

	replaceTextEdit->setText(query->getReplaceValue());

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
void QomposeReplaceDialog::initializeGUI()
{
	layout = new QGridLayout(this);

	// Create our find and replace inputs.

	findLabel = new QLabel(tr("Find expression:"), this);

	findTextEdit = new QLineEdit(this);

	replaceLabel = new QLabel(tr("Replace with:"), this);

	replaceTextEdit = new QLineEdit(this);

	// Create our options group box.

	optionsGroupBox = new QGroupBox(tr("Options"), this);
	optionsLayout = new QGridLayout(optionsGroupBox);

	wrapCheckBox = new QCheckBox(tr("Wrap around document?"), optionsGroupBox);
	wrapCheckBox->setCheckState(Qt::Checked);

	wholeWordsCheckBox = new QCheckBox(tr("Find whole words only?"), optionsGroupBox);

	caseSensitiveCheckBox = new QCheckBox(tr("Case sensitive?"), optionsGroupBox);

	reverseCheckBox = new QCheckBox(tr("Reverse search direction?"), optionsGroupBox);

	regexCheckBox = new QCheckBox(tr("Regular expression search?"), optionsGroupBox);

	optionsLayout->addWidget(wrapCheckBox, 0, 0, 1, 1);
	optionsLayout->addWidget(wholeWordsCheckBox, 1, 0, 1, 1);
	optionsLayout->addWidget(caseSensitiveCheckBox, 2, 0, 1, 1);
	optionsLayout->addWidget(reverseCheckBox, 3, 0, 1, 1);
	optionsLayout->addWidget(regexCheckBox, 4, 0, 1, 1);
	optionsLayout->setRowStretch(5, 1);
	optionsGroupBox->setLayout(optionsLayout);

	// Create our buttons widget.

	buttonsWidget = new QWidget(this);
	buttonsLayout = new QGridLayout(buttonsWidget);

	replaceButton = new QPushButton(tr("&Replace"), buttonsWidget);

	findButton = new QPushButton(tr("&Find"), buttonsWidget);

	replaceSelButton = new QPushButton(tr("Replace in &Selection"), buttonsWidget);

	replaceAllButton = new QPushButton(tr("Replace &All"), buttonsWidget);

	closeButton = new QPushButton(tr("Clos&e"), buttonsWidget);

	buttonsLayout->addWidget(replaceButton, 0, 0, 1, 1);
	buttonsLayout->addWidget(findButton, 1, 0, 1, 1);
	buttonsLayout->addWidget(replaceSelButton, 2, 0, 1, 1);
	buttonsLayout->addWidget(replaceAllButton, 3, 0, 1, 1);
	buttonsLayout->addWidget(closeButton, 4, 0, 1, 1);
	buttonsLayout->setRowStretch(5, 1);
	buttonsWidget->setLayout(buttonsLayout);

	// Add our widgets to our dialog.

	layout->addWidget(findLabel, 0, 0, 1, 1);
	layout->addWidget(findTextEdit, 0, 1, 1, 1);
	layout->addWidget(replaceLabel, 1, 0, 1, 1);
	layout->addWidget(replaceTextEdit, 1, 1, 1, 1);
	layout->addWidget(optionsGroupBox, 2, 0, 1, 2);
	layout->addWidget(buttonsWidget, 0, 2, 3, 1);
	layout->setRowStretch(2, 1);
	layout->setColumnStretch(1, 1);
	setLayout(layout);

	QObject::connect( replaceButton,    SIGNAL( clicked(bool) ), this, SLOT( doReplace()          ) );
	QObject::connect( findButton,       SIGNAL( clicked(bool) ), this, SLOT( doFind()             ) );
	QObject::connect( replaceSelButton, SIGNAL( clicked(bool) ), this, SLOT( doReplaceSelection() ) );
	QObject::connect( replaceAllButton, SIGNAL( clicked(bool) ), this, SLOT( doReplaceAll()       ) );
	QObject::connect( closeButton,      SIGNAL( clicked(bool) ), this, SLOT( close()              ) );
}

/*!
 * This is a utility function which applies the currently selected options to
 * our dialog's internal replace query.
 */
void QomposeReplaceDialog::applyValues()
{
	query->setExpression(findTextEdit->text());
	query->setReplaceValue(replaceTextEdit->text());
	query->setWrapping(wrapCheckBox->checkState() == Qt::Checked);
	query->setWholeWords(wholeWordsCheckBox->checkState() == Qt::Checked);
	query->setCaseSensitive(caseSensitiveCheckBox->checkState() == Qt::Checked);
	query->setReversed(reverseCheckBox->checkState() == Qt::Checked);
	query->setRegularExpression(regexCheckBox->checkState() == Qt::Checked);
}

/*!
 * This slot handles our "replace" button being clicked by applying our
 * dialog's options to our query, and then notifying any listeners that our
 * replace action has been triggered.
 */
void QomposeReplaceDialog::doReplace()
{ /* SLOT */

	applyValues();

	Q_EMIT replaceClicked();

}

/*!
 * This slot handles our "find" button being clicked by applying our dialog's
 * options to our query, and then notifying any listeners that our find action
 * has been triggered.
 */
void QomposeReplaceDialog::doFind()
{ /* SLOT */

	applyValues();

	Q_EMIT findClicked();

}

/*!
 * This slot handles our "replace in selection" button being clicked by applying
 * our dialog's options to our query, closing our dialog and then notifying any
 * listeners that our "replace in selection" action has been triggered.
 */
void QomposeReplaceDialog::doReplaceSelection()
{ /* SLOT */

	applyValues();

	close();

	Q_EMIT replaceSelectionClicked();

}

/*!
 * This slot handles our "replace all" button being clicked by applying our dialog's
 * options to our query, closing our dialog and then notifying any listeners that
 * our "replace all" action has been triggered.
 */
void QomposeReplaceDialog::doReplaceAll()
{ /* SLOT */

	applyValues();

	close();

	Q_EMIT replaceAllClicked();

}
