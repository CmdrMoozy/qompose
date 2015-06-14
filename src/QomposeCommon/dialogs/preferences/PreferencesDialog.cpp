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

#include "PreferencesDialog.h"

#include <QGridLayout>
#include <QStackedWidget>
#include <QPushButton>
#include <QMessageBox>

#include "QomposeCommon/dialogs/preferences/PreferencesListModel.h"
#include "QomposeCommon/dialogs/preferences/PreferencesListView.h"
#include "QomposeCommon/dialogs/preferences/widgets/EditorPreferencesWidget.h"
#include "QomposeCommon/dialogs/preferences/widgets/GeneralPreferencesWidget.h"
#include "QomposeCommon/dialogs/preferences/widgets/OpenSavePreferencesWidget.h"
#include "QomposeCommon/dialogs/preferences/widgets/PreferencesScrollArea.h"
#include "QomposeCommon/util/Settings.h"

namespace qompose
{
PreferencesDialog::PreferencesDialog(QWidget *p, Qt::WindowFlags f)
        : QDialog(p, f),
          layout(nullptr),
          generalPreferencesWidget(nullptr),
          editorPreferencesWidget(nullptr),
          openSavePreferencesWidget(nullptr),
          preferencesView(nullptr),
          preferencesModel(nullptr),
          preferencesDisplayWidget(nullptr),
          buttonsWidget(nullptr),
          buttonsLayout(nullptr),
          okButton(nullptr),
          applyButton(nullptr),
          cancelButton(nullptr),
          defaultsButton(nullptr)
{
	layout = new QGridLayout(this);

	preferencesView = new PreferencesListView(this);
	createPreferencesModel();
	preferencesView->setModel(preferencesModel);

	preferencesDisplayWidget = new QStackedWidget(this);

	for(int i = 0; i < preferencesModel->rowCount(); ++i)
	{
		preferencesDisplayWidget->insertWidget(
		        i, preferencesModel->scrollWidgetAt(i));
	}

	preferencesView->setCurrentIndex(preferencesModel->index(0));
	preferencesDisplayWidget->setCurrentIndex(0);

	buttonsWidget = new QWidget(this, nullptr);
	buttonsLayout = new QGridLayout(buttonsWidget);

	okButton = new QPushButton(tr("&Ok"), buttonsWidget);

	applyButton = new QPushButton(tr("&Apply"), buttonsWidget);

	cancelButton = new QPushButton(tr("&Cancel"), buttonsWidget);

	defaultsButton =
	        new QPushButton(tr("Restore &Defaults"), buttonsWidget);

	buttonsLayout->addWidget(okButton, 0, 1, 1, 1, nullptr);
	buttonsLayout->addWidget(applyButton, 0, 2, 1, 1, nullptr);
	buttonsLayout->addWidget(cancelButton, 0, 3, 1, 1, nullptr);
	buttonsLayout->addWidget(defaultsButton, 0, 4, 1, 1, nullptr);
	buttonsLayout->setColumnStretch(0, 1);
	buttonsWidget->setLayout(buttonsLayout);

	layout->addWidget(preferencesView, 0, 0, 1, 1, nullptr);
	layout->addWidget(preferencesDisplayWidget, 0, 1, 1, 1, nullptr);
	layout->addWidget(buttonsWidget, 1, 0, 1, 2, nullptr);
	layout->setRowStretch(0, 1);
	layout->setColumnStretch(1, 1);
	setLayout(layout);

	setWindowTitle(tr("Preferences"));

	QObject::connect(preferencesView,
	                 SIGNAL(activated(const QModelIndex &)), this,
	                 SLOT(doWidgetActivated(const QModelIndex &)));

	QObject::connect(okButton, SIGNAL(clicked(bool)), this, SLOT(doOk()));
	QObject::connect(applyButton, SIGNAL(clicked(bool)), this,
	                 SLOT(doApply()));
	QObject::connect(cancelButton, SIGNAL(clicked(bool)), this,
	                 SLOT(close()));
	QObject::connect(defaultsButton, SIGNAL(clicked(bool)), this,
	                 SLOT(doDefaults()));
}

void PreferencesDialog::discardChanges()
{
	for(int i = 0; i < preferencesModel->rowCount(); ++i)
		preferencesModel->widgetAt(i)->discardChanges();
}

void PreferencesDialog::createPreferencesModel()
{
	preferencesModel = new PreferencesListModel(preferencesView);

	generalPreferencesWidget = new GeneralPreferencesWidget(this);

	editorPreferencesWidget = new EditorPreferencesWidget(this);

	openSavePreferencesWidget = new OpenSavePreferencesWidget(this);

	preferencesModel->addPreferencesWidget(generalPreferencesWidget);
	preferencesModel->addPreferencesWidget(editorPreferencesWidget);
	preferencesModel->addPreferencesWidget(openSavePreferencesWidget);
}

void PreferencesDialog::doWidgetActivated(const QModelIndex &i)
{
	preferencesDisplayWidget->setCurrentWidget(
	        preferencesModel->scrollWidgetAt(i.row()));
}

void PreferencesDialog::doOk()
{
	doApply();
	close();
}

void PreferencesDialog::doApply()
{
	for(int i = 0; i < preferencesModel->rowCount(); ++i)
		preferencesModel->widgetAt(i)->apply();
}

void PreferencesDialog::doDefaults()
{
	QMessageBox::StandardButton ret = QMessageBox::question(
	        this, tr("Confirm Resetting Defaults"),
	        tr("Are you sure you want to reset all settings to their "
	           "default values?"),
	        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

	if(ret != QMessageBox::Yes)
		return;

	Settings::instance().resetDefaults();

	for(int i = 0; i < preferencesModel->rowCount(); ++i)
		preferencesModel->widgetAt(i)->discardChanges();
}
}
