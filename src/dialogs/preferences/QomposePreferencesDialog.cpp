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

#include "QomposePreferencesDialog.h"

#include <QGridLayout>
#include <QStackedLayout>
#include <QPushButton>

#include "dialogs/preferences/QomposePreferencesListModel.h"
#include "dialogs/preferences/QomposePreferencesListView.h"
#include "dialogs/preferences/widgets/QomposeEditorPreferencesWidget.h"
#include "dialogs/preferences/widgets/QomposeGeneralPreferencesWidget.h"

QomposePreferencesDialog::QomposePreferencesDialog(QWidget *p, Qt::WindowFlags f)
	: QDialog(p, f)
{
	layout = new QGridLayout(this);
	
	createPreferencesModel();
	
	preferencesView = new QomposePreferencesListView(this);
	preferencesView->setModel(preferencesModel);
	
	preferencesDisplayWidget = new QWidget(this);
	preferencesDisplayLayout = new QStackedLayout(preferencesDisplayWidget);
	preferencesDisplayWidget->setLayout(preferencesDisplayLayout);
	
	buttonsWidget = new QWidget(this);
	buttonsLayout = new QGridLayout(buttonsWidget);
	
	okButton = new QPushButton(tr("&Ok"), buttonsWidget);
	
	applyButton = new QPushButton(tr("&Apply"), buttonsWidget);
	
	cancelButton = new QPushButton(tr("&Cancel"), buttonsWidget);
	
	defaultsButton = new QPushButton(tr("Restore &Defaults"), buttonsWidget);
	
	buttonsLayout->addWidget(okButton, 0, 1, 1, 1);
	buttonsLayout->addWidget(applyButton, 0, 2, 1, 1);
	buttonsLayout->addWidget(cancelButton, 0, 3, 1, 1);
	buttonsLayout->addWidget(defaultsButton, 0, 4, 1, 1);
	buttonsLayout->setColumnStretch(0, 1);
	buttonsWidget->setLayout(buttonsLayout);
	
	layout->addWidget(preferencesView, 0, 0, 1, 1);
	layout->addWidget(preferencesDisplayWidget, 0, 1, 1, 1);
	layout->addWidget(buttonsWidget, 1, 0, 1, 2);
	layout->setRowStretch(0, 1);
	layout->setColumnStretch(1, 1);
	setLayout(layout);
	
	setWindowTitle(tr("Qompose - Preferences"));
	
	QObject::connect( okButton,       SIGNAL( clicked(bool) ), this, SLOT( doOk()       ) );
	QObject::connect( applyButton,    SIGNAL( clicked(bool) ), this, SLOT( doApply()    ) );
	QObject::connect( cancelButton,   SIGNAL( clicked(bool) ), this, SLOT( doCancel()   ) );
	QObject::connect( defaultsButton, SIGNAL( clicked(bool) ), this, SLOT( doDefaults() ) );
}

QomposePreferencesDialog::~QomposePreferencesDialog()
{
}

void QomposePreferencesDialog::createPreferencesModel()
{
	preferencesModel = new QomposePreferencesListModel(preferencesView);
	
	generalPreferencesWidget = new QomposeGeneralPreferencesWidget(this);
	
	editorPreferencesWidget = new QomposeEditorPreferencesWidget(this);
	
	preferencesModel->addPreferencesWidget(generalPreferencesWidget);
	preferencesModel->addPreferencesWidget(editorPreferencesWidget);
}

void QomposePreferencesDialog::doOk()
{ /* SLOT */
	
	
	
}

void QomposePreferencesDialog::doApply()
{ /* SLOT */
	
	
	
}

void QomposePreferencesDialog::doCancel()
{ /* SLOT */
	
	
	
}

void QomposePreferencesDialog::doDefaults()
{ /* SLOT */
	
	
	
}
