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
#include <QStackedWidget>
#include <QPushButton>
#include <QMessageBox>

#include "QomposeCommon/dialogs/preferences/QomposePreferencesListModel.h"
#include "QomposeCommon/dialogs/preferences/QomposePreferencesListView.h"
#include "QomposeCommon/dialogs/preferences/widgets/QomposeEditorPreferencesWidget.h"
#include "QomposeCommon/dialogs/preferences/widgets/QomposeGeneralPreferencesWidget.h"
#include "QomposeCommon/dialogs/preferences/widgets/QomposeOpenSavePreferencesWidget.h"
#include "QomposeCommon/dialogs/preferences/widgets/QomposePreferencesScrollArea.h"
#include "QomposeCommon/util/QomposeSettings.h"

/*!
 * This is our default constructor, which creates a new instance of our
 * preferences dialog.
 *
 * \param s The settings instance to use to persist settings.
 * \param p The parent widget for this dialog.
 * \param f The window flags for this dialog.
 */
QomposePreferencesDialog::QomposePreferencesDialog(QomposeSettings *s,
	QWidget *p, Qt::WindowFlags f)
	: QDialog(p, f), settings(s), layout(nullptr),
		generalPreferencesWidget(nullptr),
		editorPreferencesWidget(nullptr),
		openSavePreferencesWidget(nullptr),
		preferencesView(nullptr), preferencesModel(nullptr),
		preferencesDisplayWidget(nullptr), buttonsWidget(nullptr),
		buttonsLayout(nullptr), okButton(nullptr),
		applyButton(nullptr), cancelButton(nullptr),
		defaultsButton(nullptr)
{
	layout = new QGridLayout(this);

	preferencesView = new QomposePreferencesListView(this);
	createPreferencesModel();
	preferencesView->setModel(preferencesModel);

	preferencesDisplayWidget = new QStackedWidget(this);

	for(int i = 0; i < preferencesModel->rowCount(); ++i)
	{
		preferencesDisplayWidget->insertWidget(i,
			preferencesModel->scrollWidgetAt(i));
	}

	preferencesView->setCurrentIndex(preferencesModel->index(0));
	preferencesDisplayWidget->setCurrentIndex(0);

	buttonsWidget = new QWidget(this, nullptr);
	buttonsLayout = new QGridLayout(buttonsWidget);

	okButton = new QPushButton(tr("&Ok"), buttonsWidget);

	applyButton = new QPushButton(tr("&Apply"), buttonsWidget);

	cancelButton = new QPushButton(tr("&Cancel"), buttonsWidget);

	defaultsButton = new QPushButton(tr("Restore &Defaults"), buttonsWidget);

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

	QObject::connect( preferencesView, SIGNAL( activated(const QModelIndex &) ),
		this, SLOT( doWidgetActivated(const QModelIndex &) ) );

	QObject::connect( okButton,       SIGNAL( clicked(bool) ), this, SLOT( doOk()       ) );
	QObject::connect( applyButton,    SIGNAL( clicked(bool) ), this, SLOT( doApply()    ) );
	QObject::connect( cancelButton,   SIGNAL( clicked(bool) ), this, SLOT( close()      ) );
	QObject::connect( defaultsButton, SIGNAL( clicked(bool) ), this, SLOT( doDefaults() ) );
}

/*!
 * This is our default destructor, which cleans up & destroys our dialog.
 */
QomposePreferencesDialog::~QomposePreferencesDialog()
{
}

/*!
 * This function discards any changes that have been made to any of our
 * preferences widgets by instructing each widget in our model to discard
 * its changes.
 */
void QomposePreferencesDialog::discardChanges()
{
	for(int i = 0; i < preferencesModel->rowCount(); ++i)
		preferencesModel->widgetAt(i)->discardChanges();
}

/*!
 * This function initializes our preferences model by creating the model
 * as well as all of the widgets that will be placed inside of it.
 */
void QomposePreferencesDialog::createPreferencesModel()
{
	preferencesModel = new QomposePreferencesListModel(preferencesView);

	generalPreferencesWidget = new QomposeGeneralPreferencesWidget(settings, this);

	editorPreferencesWidget = new QomposeEditorPreferencesWidget(settings, this);

	openSavePreferencesWidget = new QomposeOpenSavePreferencesWidget(settings, this);

	preferencesModel->addPreferencesWidget(generalPreferencesWidget);
	preferencesModel->addPreferencesWidget(editorPreferencesWidget);
	preferencesModel->addPreferencesWidget(openSavePreferencesWidget);
}

/*!
 * This function handles a new widget being activated in our list view of
 * preferences widgets by displaying the associated widget in our dialog's
 * display area.
 *
 * \param i The model index (i.e., the row) of the widget that was activated.
 */
void QomposePreferencesDialog::doWidgetActivated(const QModelIndex &i)
{ /* SLOT */

	preferencesDisplayWidget->setCurrentWidget(
		preferencesModel->scrollWidgetAt(i.row()));

}

/*!
 * This function handles our OK button being clicked by applying all of our
 * preferences values, and then closing our dialog.
 */
void QomposePreferencesDialog::doOk()
{ /* SLOT */

	doApply();

	close();

}

/*!
 * This function handles our apply button being clicked by instructing each of
 * the preferences widgets in our model to apply their respective settings.
 */
void QomposePreferencesDialog::doApply()
{ /* SLOT */

	for(int i = 0; i < preferencesModel->rowCount(); ++i)
		preferencesModel->widgetAt(i)->apply();

}

/*!
 * This function handles our defaults button being clicked by resetting all of
 * our preferences values back to their defaults, and then instructing each of
 * the preferences widgets in our model to reload the settings from the settings
 * instance.
 */
void QomposePreferencesDialog::doDefaults()
{ /* SLOT */

	QMessageBox::StandardButton ret = QMessageBox::question(this,
		tr("Confirm Resetting Defaults"),
		tr("Are you sure you want to reset all settings to their default values?"),
		QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

	if(ret != QMessageBox::Yes)
		return;

	settings->resetDefaults();

	for(int i = 0; i < preferencesModel->rowCount(); ++i)
		preferencesModel->widgetAt(i)->discardChanges();

}
