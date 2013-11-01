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

#include "QomposeStatusBar.h"

#include <QGridLayout>
#include <QLabel>

QomposeStatusBar::QomposeStatusBar(QWidget *p)
	: QStatusBar(p)
{
	statusWidget = new QWidget(this);
	
	statusLayout = new QGridLayout(statusWidget);
	statusLayout->setContentsMargins(0, 0, 0, 0);
	statusLayout->setSpacing(5);
	
	tabPathLabel = new QLabel(statusWidget);
	
	statusLayout->addWidget(tabPathLabel, 0, 1, 1, 1);
	statusLayout->setColumnStretch(0, 1);
	statusWidget->setLayout(statusLayout);
	
	addPermanentWidget(statusWidget, 1);
}

QomposeStatusBar::~QomposeStatusBar()
{
}

void QomposeStatusBar::setCurrentTabPath(const QString &p)
{
	tabPathLabel->setText(p);
}
