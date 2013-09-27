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

QomposeReplaceDialog::QomposeReplaceDialog(QWidget *p, Qt::WindowFlags f)
	: QDialog(p, f)
{
	setWindowTitle(tr("Replace"));
	
	initializeGUI();
}

QomposeReplaceDialog::~QomposeReplaceDialog()
{
}

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
	
	optionsLayout->addWidget(wrapCheckBox, 0, 0, 1, 1);
	optionsLayout->addWidget(wholeWordsCheckBox, 1, 0, 1, 1);
	optionsLayout->addWidget(caseSensitiveCheckBox, 2, 0, 1, 1);
	optionsLayout->addWidget(reverseCheckBox, 3, 0, 1, 1);
	optionsLayout->setRowStretch(4, 1);
	optionsGroupBox->setLayout(optionsLayout);
	
	// Create our buttons widget.
	
	buttonsWidget = new QWidget(this);
	buttonsLayout = new QGridLayout(buttonsWidget);
	
	replaceButton = new QPushButton(tr("&Replace"), buttonsWidget);
	
	findButton = new QPushButton(tr("&Find"), buttonsWidget);
	
	replaceInSelectionButton = new QPushButton(tr("Replace in &Selection"), buttonsWidget);
	
	replaceAllButton = new QPushButton(tr("Replace &All"), buttonsWidget);
	
	closeButton = new QPushButton(tr("Clos&e"), buttonsWidget);
	
	buttonsLayout->addWidget(replaceButton, 0, 0, 1, 1);
	buttonsLayout->addWidget(findButton, 1, 0, 1, 1);
	buttonsLayout->addWidget(replaceInSelectionButton, 2, 0, 1, 1);
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
}
