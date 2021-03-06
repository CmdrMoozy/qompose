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

#include "EncodingDialog.h"

#include <QByteArray>
#include <QGridLayout>
#include <QLabel>
#include <QList>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include <QTextCodec>

#include "QomposeCommon/Defines.h"

namespace qompose
{
EncodingDialog::EncodingDialog(QWidget *p, Qt::WindowFlags f, const QString &d,
                               const QString &m, const QString &t)
        : QDialog(p, f),
          encoding(QString()),
          layout(nullptr),
          messageLabel(nullptr),
          encodingList(nullptr),
          buttonsWidget(nullptr),
          buttonsLayout(nullptr),
          selectButton(nullptr),
          cancelButton(nullptr)
{
	layout = new QGridLayout(this);

	messageLabel = new QLabel(m, this, nullptr);

	encodingList = new QListWidget(this);

	QList<QByteArray> codecs = QTextCodec::availableCodecs();

	for(int i = 0; i < codecs.size(); ++i)
		encodingList->addItem(QString(codecs.at(i)));

	QList<QListWidgetItem *> defaultEncoding =
	        encodingList->findItems(d, Qt::MatchExactly);

	if(defaultEncoding.size() == 1)
		encodingList->setCurrentItem(defaultEncoding.first());

	buttonsWidget = new QWidget(this, nullptr);
	buttonsLayout = new QGridLayout(buttonsWidget);

	selectButton = new QPushButton(tr("&Select"), buttonsWidget);

	cancelButton = new QPushButton(tr("&Cancel"), buttonsWidget);

	buttonsLayout->addWidget(selectButton, 0, 1, 1, 1, nullptr);
	buttonsLayout->addWidget(cancelButton, 0, 2, 1, 1, nullptr);
	buttonsLayout->setColumnStretch(0, 1);
	buttonsWidget->setLayout(buttonsLayout);

	layout->addWidget(messageLabel, 0, 0, 1, 1, nullptr);
	layout->addWidget(encodingList, 1, 0, 1, 1, nullptr);
	layout->addWidget(buttonsWidget, 2, 0, 1, 1, nullptr);
	setLayout(layout);

	QObject::connect(selectButton, SIGNAL(clicked(bool)), this,
	                 SLOT(doSelectClicked()));
	QObject::connect(cancelButton, SIGNAL(clicked(bool)), this,
	                 SLOT(doCancelClicked()));

	setModal(true);
	setWindowTitle(t);
}

QString EncodingDialog::promptEncoding(QWidget *p, const QString &d,
                                       const QString &m, const QString &t)
{
	EncodingDialog dialog(p, nullptr, d, m, t);
	dialog.exec();
	return dialog.getSelectedEncoding();
}

QString EncodingDialog::getSelectedEncoding() const
{
	return encoding;
}

void EncodingDialog::doSelectClicked()
{
	QList<QListWidgetItem *> selected = encodingList->selectedItems();

	if(selected.size() == 1)
		encoding = selected.first()->text();
	else
		encoding = QString();

	done(0);
}

void EncodingDialog::doCancelClicked()
{
	encoding = QString();
	done(0);
}
}
