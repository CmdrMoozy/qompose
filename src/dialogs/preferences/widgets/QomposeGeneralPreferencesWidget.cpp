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

#include "QomposeGeneralPreferencesWidget.h"

#include <QGridLayout>
#include <QCheckBox>
#include <QLabel>
#include <QSpinBox>

#include "gui/QomposeGUIUtils.h"

QomposeGeneralPreferencesWidget::QomposeGeneralPreferencesWidget(QWidget *p, Qt::WindowFlags f)
	: QomposePreferencesWidget(p, f)
{
	setPreferencesIcon(QomposeGUIUtils::getIconFromTheme("preferences-other"));
	setPreferencesTitle(tr("General"));
	
	initializeGUI();
}

QomposeGeneralPreferencesWidget::~QomposeGeneralPreferencesWidget()
{
}

void QomposeGeneralPreferencesWidget::initializeGUI()
{
	layout = new QGridLayout(this);
	
	statusBarCheckBox = new QCheckBox(tr("Show Status Bar"), this);
	
	recentListSizeLabel = new QLabel(tr("Recently Opened List Size"), this);
	
	recentListSizeSpinBox = new QSpinBox(this);
	recentListSizeSpinBox->setMinimum(0);
	recentListSizeSpinBox->setMaximum(50);
	
	saveWindowAttribsCheckBox = new QCheckBox(tr("Save Window Attributes on Exit"), this);
	
	layout->addWidget( statusBarCheckBox,         0, 0, 1, 1 );
	layout->addWidget( recentListSizeLabel,       1, 0, 1, 1 );
	layout->addWidget( recentListSizeSpinBox,     1, 1, 1, 1 );
	layout->addWidget( saveWindowAttribsCheckBox, 2, 0, 1, 1 );
	
	layout->setColumnStretch(0, 1);
	layout->setRowStretch(3, 1);
	
	setLayout(layout);
}
