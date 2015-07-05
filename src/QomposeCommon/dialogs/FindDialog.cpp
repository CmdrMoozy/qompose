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

#include "FindDialog.h"

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QGroupBox>
#include <QCheckBox>
#include <QWidget>
#include <QPushButton>
#include <QShowEvent>

#include "QomposeCommon/Defines.h"

namespace qompose
{
FindDialog::FindDialog(QWidget *p, Qt::WindowFlags f)
        : QDialog(p, f),
          query(),
          layout(nullptr),
          findLabel(nullptr),
          findTextEdit(nullptr),
          optionsGroupBox(nullptr),
          optionsLayout(nullptr),
          wrapCheckBox(nullptr),
          wholeWordsCheckBox(nullptr),
          caseSensitiveCheckBox(nullptr),
          regexCheckBox(nullptr),
          buttonsWidget(nullptr),
          buttonsLayout(nullptr),
          findButton(nullptr),
          closeButton(nullptr)
{
	setWindowTitle(tr("Find"));

	initializeGUI();

	setMinimumWidth(400);
}

editor::search::FindQuery FindDialog::getQuery() const
{
	return query;
}

void FindDialog::showEvent(QShowEvent *e)
{
	// Setup our line text edit.

	findTextEdit->setFocus();
	findTextEdit->setText(query.expression);
	findTextEdit->selectAll();

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

void FindDialog::initializeGUI()
{
	layout = new QGridLayout(this);

	// Create our find expression inputs.

	findLabel = new QLabel(tr("Find expression:"), this, nullptr);

	findTextEdit = new QLineEdit();

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

	QObject::connect(findButton, SIGNAL(clicked(bool)), this,
	                 SLOT(doFind()));
	QObject::connect(closeButton, SIGNAL(clicked(bool)), this,
	                 SLOT(close()));
}

void FindDialog::doFind()
{
	query.expression = findTextEdit->text();
	query.wrap = wrapCheckBox->checkState() == Qt::Checked;
	query.wholeWords = wholeWordsCheckBox->checkState() == Qt::Checked;
	query.caseSensitive =
	        caseSensitiveCheckBox->checkState() == Qt::Checked;
	query.isRegex = regexCheckBox->checkState() == Qt::Checked;

	Q_EMIT accepted();
	close();
}
}
