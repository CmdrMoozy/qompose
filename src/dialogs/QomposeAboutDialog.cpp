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

#include "QomposeAboutDialog.h"

#include <QGridLayout>
#include <QTabWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QFile>
#include <QTextStream>
#include <QFont>
#include <QLabel>
#include <QWidget>
#include <QPixmap>

/*!
 * This is our default constructor, which creates a new about dialog instance
 * and loads the contents from our resources into its UI.
 *
 * \param p This dialog's parent widget.
 * \param f The window flags to use for this dialog.
 */
QomposeAboutDialog::QomposeAboutDialog(QWidget *p, Qt::WindowFlags f)
	: QDialog(p, f)
{
	setWindowTitle(tr("About Qompose"));
	
	initializeGUI();
	
	resize(500, 500);
}

/*!
 * This is our default destructor, which cleans up & destroys our dialog.
 */
QomposeAboutDialog::~QomposeAboutDialog()
{
}

/*!
 * This is a utility function which initializes our GUI elements and
 * adds them to our layout.
 */
void QomposeAboutDialog::initializeGUI()
{
	layout = new QGridLayout(this);
	
	tabs = new QTabWidget(this);
	
	// Create our Qompose tab.
	
	qomposeTab = new QWidget(tabs);
	qomposeTabLayout = new QGridLayout(qomposeTab);
	
	qomposeIconLabel = new QLabel(qomposeTab);
	qomposeIconLabel->setPixmap(QPixmap(":/icons/qompose.png"));
	qomposeIconLabel->setScaledContents(true);
	qomposeIconLabel->resize(32, 32);
	
	qomposeLabel = new QLabel(tr("Qompose"), qomposeTab);
	
	QFont largeFont = qomposeLabel->font();
	largeFont.setPointSize(24);
	
	qomposeLabel->setFont(largeFont);
	
	qomposeDescriptionLabel = new QLabel(tr("A simple programmer's "
		"text editor."), qomposeTab);
	
	qomposeTextEdit = new QTextEdit(qomposeTab);
	qomposeTextEdit->setLineWrapMode(QTextEdit::NoWrap);
	qomposeTextEdit->setReadOnly(true);
	
	qomposeTabLayout->addWidget(qomposeIconLabel, 0, 0, 2, 1);
	qomposeTabLayout->addWidget(qomposeLabel, 0, 1, 1, 1);
	qomposeTabLayout->addWidget(qomposeDescriptionLabel, 1, 1, 1, 1);
	qomposeTabLayout->addWidget(qomposeTextEdit, 2, 0, 1, 3);
	qomposeTabLayout->setColumnStretch(1, 1);
	qomposeTabLayout->setRowStretch(2, 1);
	qomposeTab->setLayout(qomposeTabLayout);
	
	// Create our license tab.
	
	QFont monospace("Monospace");
	monospace.setPointSize(9);
	monospace.setStyleHint(QFont::TypeWriter);
	
	licenseTextEdit = new QTextEdit(tabs);
	licenseTextEdit->setLineWrapMode(QTextEdit::NoWrap);
	licenseTextEdit->setReadOnly(true);
	licenseTextEdit->setCurrentFont(monospace);
	loadLicense();
	
	// Add our tabs and our buttons to our layout.
	
	tabs->addTab(qomposeTab, tr("Qompose"));
	tabs->addTab(licenseTextEdit, tr("License"));
	
	closeButton = new QPushButton(tr("Clos&e"), this);
	
	layout->addWidget(tabs, 0, 0, 1, 1);
	layout->addWidget(closeButton, 1, 0, 1, 1);
	layout->setRowStretch(0, 1);
	setLayout(layout);
	
	QObject::connect( closeButton, SIGNAL( clicked(bool) ), this, SLOT( close() ) );
}

/*!
 * This is a utility function which loads the body of our license file,
 * and places its contents in our license text edit.
 */
void QomposeAboutDialog::loadLicense()
{
	licenseTextEdit->clear();
	
	QFile f(":/data/gpl.txt");
	
	if(!f.open(QIODevice::ReadOnly))
		return;
	
	QTextStream reader(&f);
	licenseTextEdit->setText(reader.readAll());
	
	f.close();
}
