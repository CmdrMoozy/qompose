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
	layout = new QGridLayout(this);
	
	tabs = new QTabWidget(this);
	
	QFont monospace("Monospace");
	monospace.setPointSize(9);
	monospace.setStyleHint(QFont::TypeWriter);
	
	licenseTextEdit = new QTextEdit(tabs);
	licenseTextEdit->setLineWrapMode(QTextEdit::NoWrap);
	licenseTextEdit->setReadOnly(true);
	licenseTextEdit->setCurrentFont(monospace);
	loadLicense();
	
	tabs->addTab(licenseTextEdit, tr("License"));
	
	closeButton = new QPushButton(tr("Clos&e"), this);
	
	layout->addWidget(tabs, 0, 0, 1, 1);
	layout->addWidget(closeButton, 1, 0, 1, 1);
	layout->setRowStretch(0, 1);
	setLayout(layout);
	
	QObject::connect( closeButton, SIGNAL( clicked(bool) ), this, SLOT( close() ) );
	
	setWindowTitle(tr("About Qompose"));
	resize(500, 500);
}

/*!
 * This is our default destructor, which cleans up & destroys our dialog.
 */
QomposeAboutDialog::~QomposeAboutDialog()
{
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
