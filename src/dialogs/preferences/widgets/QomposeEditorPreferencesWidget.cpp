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

#include "QomposeEditorPreferencesWidget.h"

#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QCheckBox>
#include <QSpinBox>

#include "gui/QomposeColorPickerButton.h"
#include "gui/QomposeFontPickerButton.h"
#include "gui/QomposeGUIUtils.h"
#include "util/QomposeSettings.h"

QomposeEditorPreferencesWidget::QomposeEditorPreferencesWidget(QomposeSettings *s,
	QWidget *p, Qt::WindowFlags f)
	: QomposePreferencesWidget(s, p, f)
{
	setPreferencesIcon(QomposeGUIUtils::getIconFromTheme("accessories-text-editor"));
	setPreferencesTitle(tr("Editor"));
	
	initializeGUI();
}

QomposeEditorPreferencesWidget::~QomposeEditorPreferencesWidget()
{
}

void QomposeEditorPreferencesWidget::discardChanges()
{
	// Show Gutter
	
	bool showGutter = getSettings()->getSetting(
		"show-gutter").toBool();
	
	showGutterCheckBox->setCheckState(showGutter ?
		Qt::Checked : Qt::Unchecked);
	
	// Editor Font
	
	editorFontButton->setSelectedFont(getSettings()->getSetting(
		"editor-font").value<QFont>());
	
	// Tab Width
	
	tabWidthSpinBox->setValue(getSettings()->getSetting(
		"editor-tab-width").toInt());
	
	// Editor Foreground
	
	editorFGButton->setSelectedColor(getSettings()->getSetting(
		"editor-foreground").value<QColor>());
	
	// Editor Background
	
	editorBGButton->setSelectedColor(getSettings()->getSetting(
		"editor-background").value<QColor>());
	
	// Current Line Background
	
	currentLineBGButton->setSelectedColor(getSettings()->getSetting(
		"editor-current-line").value<QColor>());
	
	// Gutter Foreground
	
	gutterFGButton->setSelectedColor(getSettings()->getSetting(
		"gutter-foreground").value<QColor>());
	
	// Gutter Background
	
	gutterBGButton->setSelectedColor(getSettings()->getSetting(
		"gutter-background").value<QColor>());
}

void QomposeEditorPreferencesWidget::initializeGUI()
{
	layout = new QGridLayout(this);
	
	generalGroupBox = new QGroupBox(tr("General"), this);
	generalLayout = new QGridLayout(generalGroupBox);
	
	showGutterCheckBox = new QCheckBox(tr("Show Gutter"), generalGroupBox);
	
	editorFontLabel = new QLabel(tr("Editor Font"), generalGroupBox);
	
	editorFontButton = new QomposeFontPickerButton(generalGroupBox);
	
	tabWidthLabel = new QLabel(tr("Tab Width"), generalGroupBox);
	
	tabWidthSpinBox = new QSpinBox(generalGroupBox);
	tabWidthSpinBox->setMinimum(2);
	tabWidthSpinBox->setMaximum(24);
	
	generalLayout->addWidget( showGutterCheckBox, 0, 0, 1, 1 );
	generalLayout->addWidget( editorFontLabel,    1, 0, 1, 1 );
	generalLayout->addWidget( editorFontButton,   1, 1, 1, 1 );
	generalLayout->addWidget( tabWidthLabel,      2, 0, 1, 1 );
	generalLayout->addWidget( tabWidthSpinBox,    2, 1, 1, 1 );
	
	generalLayout->setRowStretch(3, 1);
	generalLayout->setColumnStretch(0, 1);
	
	generalGroupBox->setLayout(generalLayout);
	
	colorsGroupBox = new QGroupBox(tr("Colors"), this);
	colorsLayout = new QGridLayout(colorsGroupBox);
	
	editorFGLabel = new QLabel(tr("Editor Foreground"), colorsGroupBox);
	editorFGButton = new QomposeColorPickerButton(colorsGroupBox);
	
	editorBGLabel = new QLabel(tr("Editor Background"), colorsGroupBox);
	editorBGButton = new QomposeColorPickerButton(colorsGroupBox);
	
	currentLineBGLabel = new QLabel(tr("Current Line Background"), colorsGroupBox);
	currentLineBGButton = new QomposeColorPickerButton(colorsGroupBox);
	
	gutterFGLabel = new QLabel(tr("Gutter Foreground"), colorsGroupBox);
	gutterFGButton = new QomposeColorPickerButton(colorsGroupBox);
	
	gutterBGLabel = new QLabel(tr("Gutter Background"), colorsGroupBox);
	gutterBGButton = new QomposeColorPickerButton(colorsGroupBox);
	
	colorsLayout->addWidget( editorFGLabel,       0, 0, 1, 1 );
	colorsLayout->addWidget( editorFGButton,      0, 1, 1, 1 );
	colorsLayout->addWidget( editorBGLabel,       1, 0, 1, 1 );
	colorsLayout->addWidget( editorBGButton,      1, 1, 1, 1 );
	colorsLayout->addWidget( currentLineBGLabel,  2, 0, 1, 1 );
	colorsLayout->addWidget( currentLineBGButton, 2, 1, 1, 1 );
	colorsLayout->addWidget( gutterFGLabel,       3, 0, 1, 1 );
	colorsLayout->addWidget( gutterFGButton,      3, 1, 1, 1 );
	colorsLayout->addWidget( gutterBGLabel,       4, 0, 1, 1 );
	colorsLayout->addWidget( gutterBGButton,      4, 1, 1, 1 );
	
	colorsLayout->setRowStretch(5, 1);
	colorsLayout->setColumnStretch(0, 1);
	
	colorsGroupBox->setLayout(colorsLayout);
	
	layout->addWidget(generalGroupBox, 1, 0, 1, 1);
	layout->addWidget(colorsGroupBox, 2, 0, 1, 1);
	
	layout->setRowStretch(3, 1);
	
	setLayout(layout);
}
