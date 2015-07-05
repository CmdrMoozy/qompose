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

#include "ReplaceDialog.h"

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QGroupBox>
#include <QCheckBox>
#include <QWidget>
#include <QPushButton>
#include <QShowEvent>

namespace qompose
{
ReplaceDialog::ReplaceDialog(QWidget *p, Qt::WindowFlags f)
        : QDialog(p, f),
          query(),
          layout(nullptr),
          findLabel(nullptr),
          findTextEdit(nullptr),
          replaceLabel(nullptr),
          replaceTextEdit(nullptr),
          optionsGroupBox(nullptr),
          optionsLayout(nullptr),
          wrapCheckBox(nullptr),
          wholeWordsCheckBox(nullptr),
          caseSensitiveCheckBox(nullptr),
          regexCheckBox(nullptr),
          buttonsWidget(nullptr),
          buttonsLayout(nullptr),
          replaceButton(nullptr),
          findButton(nullptr),
          replaceSelButton(nullptr),
          replaceAllButton(nullptr),
          closeButton(nullptr)
{
	setWindowTitle(tr("Replace"));

	initializeGUI();

	setMinimumWidth(450);
}

editor::search::ReplaceQuery ReplaceDialog::getQuery() const
{
	return query;
}

void ReplaceDialog::showEvent(QShowEvent *e)
{
	// Setup our line text edit.

	findTextEdit->setFocus();
	findTextEdit->setText(query.expression);
	findTextEdit->selectAll();

	replaceTextEdit->setText(query.replaceValue);

	wrapCheckBox->setCheckState(query.wrap ? Qt::Checked : Qt::Unchecked);

	wholeWordsCheckBox->setCheckState(query.wholeWords ? Qt::Checked
	                                                   : Qt::Unchecked);

	caseSensitiveCheckBox->setCheckState(
	        query.caseSensitive ? Qt::Checked : Qt::Unchecked);

	regexCheckBox->setCheckState(query.isRegex ? Qt::Checked
	                                           : Qt::Unchecked);

	// Bring our dialog to the front.

	raise();

	// Let our parent class do its thing.

	QDialog::showEvent(e);
}

void ReplaceDialog::initializeGUI()
{
	layout = new QGridLayout(this);

	// Create our find and replace inputs.

	findLabel = new QLabel(tr("Find expression:"), this, nullptr);

	findTextEdit = new QLineEdit(this);

	replaceLabel = new QLabel(tr("Replace with:"), this, nullptr);

	replaceTextEdit = new QLineEdit(this);

	// Create our options group box.

	optionsGroupBox = new QGroupBox(tr("Options"), this);
	optionsLayout = new QGridLayout(optionsGroupBox);

	wrapCheckBox =
	        new QCheckBox(tr("Wrap around document?"), optionsGroupBox);
	wrapCheckBox->setCheckState(Qt::Checked);

	wholeWordsCheckBox =
	        new QCheckBox(tr("Find whole words only?"), optionsGroupBox);

	caseSensitiveCheckBox =
	        new QCheckBox(tr("Case sensitive?"), optionsGroupBox);

	regexCheckBox = new QCheckBox(tr("Regular expression search?"),
	                              optionsGroupBox);

	optionsLayout->addWidget(wrapCheckBox, 0, 0, 1, 1, nullptr);
	optionsLayout->addWidget(wholeWordsCheckBox, 1, 0, 1, 1, nullptr);
	optionsLayout->addWidget(caseSensitiveCheckBox, 2, 0, 1, 1, nullptr);
	optionsLayout->addWidget(regexCheckBox, 3, 0, 1, 1, nullptr);
	optionsLayout->setRowStretch(4, 1);
	optionsGroupBox->setLayout(optionsLayout);

	// Create our buttons widget.

	buttonsWidget = new QWidget(this, nullptr);
	buttonsLayout = new QGridLayout(buttonsWidget);

	replaceButton = new QPushButton(tr("&Replace"), buttonsWidget);

	findButton = new QPushButton(tr("&Find"), buttonsWidget);

	replaceSelButton =
	        new QPushButton(tr("Replace in &Selection"), buttonsWidget);

	replaceAllButton = new QPushButton(tr("Replace &All"), buttonsWidget);

	closeButton = new QPushButton(tr("Clos&e"), buttonsWidget);

	buttonsLayout->addWidget(replaceButton, 0, 0, 1, 1, nullptr);
	buttonsLayout->addWidget(findButton, 1, 0, 1, 1, nullptr);
	buttonsLayout->addWidget(replaceSelButton, 2, 0, 1, 1, nullptr);
	buttonsLayout->addWidget(replaceAllButton, 3, 0, 1, 1, nullptr);
	buttonsLayout->addWidget(closeButton, 4, 0, 1, 1, nullptr);
	buttonsLayout->setRowStretch(5, 1);
	buttonsWidget->setLayout(buttonsLayout);

	// Add our widgets to our dialog.

	layout->addWidget(findLabel, 0, 0, 1, 1, nullptr);
	layout->addWidget(findTextEdit, 0, 1, 1, 1, nullptr);
	layout->addWidget(replaceLabel, 1, 0, 1, 1, nullptr);
	layout->addWidget(replaceTextEdit, 1, 1, 1, 1, nullptr);
	layout->addWidget(optionsGroupBox, 2, 0, 1, 2, nullptr);
	layout->addWidget(buttonsWidget, 0, 2, 3, 1, nullptr);
	layout->setRowStretch(2, 1);
	layout->setColumnStretch(1, 1);
	setLayout(layout);

	QObject::connect(replaceButton, SIGNAL(clicked(bool)), this,
	                 SLOT(doReplace()));
	QObject::connect(findButton, SIGNAL(clicked(bool)), this,
	                 SLOT(doFind()));
	QObject::connect(replaceSelButton, SIGNAL(clicked(bool)), this,
	                 SLOT(doReplaceSelection()));
	QObject::connect(replaceAllButton, SIGNAL(clicked(bool)), this,
	                 SLOT(doReplaceAll()));
	QObject::connect(closeButton, SIGNAL(clicked(bool)), this,
	                 SLOT(close()));
}

void ReplaceDialog::applyValues()
{
	query.expression = findTextEdit->text();
	query.replaceValue = replaceTextEdit->text();
	query.wrap = wrapCheckBox->checkState() == Qt::Checked;
	query.wholeWords = wholeWordsCheckBox->checkState() == Qt::Checked;
	query.caseSensitive =
	        caseSensitiveCheckBox->checkState() == Qt::Checked;
	query.isRegex = regexCheckBox->checkState() == Qt::Checked;
}

void ReplaceDialog::doReplace()
{
	applyValues();
	Q_EMIT replaceClicked();
}

void ReplaceDialog::doFind()
{
	applyValues();
	Q_EMIT findClicked();
}

void ReplaceDialog::doReplaceSelection()
{
	applyValues();
	close();
	Q_EMIT replaceSelectionClicked();
}

void ReplaceDialog::doReplaceAll()
{
	applyValues();
	close();
	Q_EMIT replaceAllClicked();
}
}
