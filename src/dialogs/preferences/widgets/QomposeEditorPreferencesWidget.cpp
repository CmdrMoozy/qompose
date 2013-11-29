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

/*!
 * This is our default constructor, which creates a new instance of our
 * editor preferences widget.
 *
 * \param s The settings instance to use to save preferences values.
 * \param p The parent widget to use for this widget.
 * \param f The window flags to use for this widget.
 */
QomposeEditorPreferencesWidget::QomposeEditorPreferencesWidget(QomposeSettings *s,
	QWidget *p, Qt::WindowFlags f)
	: QomposePreferencesWidget(s, p, f)
{
	setPreferencesIcon(QomposeGUIUtils::getIconFromTheme("accessories-text-editor"));
	setPreferencesTitle(tr("Editor"));
	
	initializeGUI();
}

/*!
 * This is our default destructor, which cleans up & destroys our widget.
 */
QomposeEditorPreferencesWidget::~QomposeEditorPreferencesWidget()
{
}

/*!
 * We implement our superclass's apply() function to save our various editor
 * preferences values using our settings instance.
 */
void QomposeEditorPreferencesWidget::apply()
{
	// Show Gutter
	
	getSettings()->setSetting("show-gutter", QVariant(
		showGutterCheckBox->checkState() == Qt::Checked));
	
	// Editor Font
	
	getSettings()->setSetting("editor-font", QVariant(
		editorFontButton->getSelectedFont()));
	
	// Tab Width
	
	getSettings()->setSetting("editor-tab-width", QVariant(
		tabWidthSpinBox->value()));
	
	// Wrap Guide Visible
	
	getSettings()->setSetting("editor-wrap-guide-visible", QVariant(
		lineWrapGuideCheckBox->checkState() == Qt::Checked));
	
	// Wrap Guide Width
	
	getSettings()->setSetting("editor-wrap-guide-width", QVariant(
		lineWrapGuideWidthSpinBox->value()));
	
	// Wrap Guide Color
	
	getSettings()->setSetting("editor-wrap-guide-color", QVariant(
		lineWrapGuideColorButton->getSelectedColor()));
	
	// Editor Foreground
	
	getSettings()->setSetting("editor-foreground", QVariant(
		editorFGButton->getSelectedColor()));
	
	// Editor Background
	
	getSettings()->setSetting("editor-background", QVariant(
		editorBGButton->getSelectedColor()));
	
	// Current Line Background
	
	getSettings()->setSetting("editor-current-line", QVariant(
		currentLineBGButton->getSelectedColor()));
	
	// Gutter Foreground
	
	getSettings()->setSetting("gutter-foreground", QVariant(
		gutterFGButton->getSelectedColor()));
	
	// Gutter Background
	
	getSettings()->setSetting("gutter-background", QVariant(
		gutterBGButton->getSelectedColor()));
}

/*!
 * We implement our superclass's discardChanges() function to discard
 * any preferences changes that may have been made to our widget by
 * reloading the existing values from our settings instance.
 */
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
	
	// Wrap Guide Visible
	
	bool showLineWrapGuide = getSettings()->getSetting(
		"editor-wrap-guide-visible").toBool();
	
	lineWrapGuideCheckBox->setCheckState(showLineWrapGuide ?
		Qt::Checked : Qt::Unchecked);
	
	// Wrap Guide Width
	
	lineWrapGuideWidthSpinBox->setValue(getSettings()->getSetting(
		"editor-wrap-guide-width").toInt());
	
	// Wrap Guide Color
	
	lineWrapGuideColorButton->setSelectedColor(getSettings()->getSetting(
		"editor-wrap-guide-color").value<QColor>());
	
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

/*!
 * This function initializes our widget's GUI by creating the various widgets
 * we contain, and adding them to our layout.
 */
void QomposeEditorPreferencesWidget::initializeGUI()
{
	layout = new QGridLayout(this);
	
	// Initialize our general group box.
	
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
	
	// Initialize our line wrap guide group box.
	
	lineWrapGuideGroupBox = new QGroupBox(tr("Line Wrap Guide"), this);
	lineWrapGuideLayout = new QGridLayout(lineWrapGuideGroupBox);
	
	lineWrapGuideCheckBox = new QCheckBox(tr("Show Guide"), lineWrapGuideGroupBox);
	
	lineWrapGuideWidthLabel = new QLabel(tr("Character Width"), lineWrapGuideGroupBox);
	
	lineWrapGuideWidthSpinBox = new QSpinBox(lineWrapGuideGroupBox);
	lineWrapGuideWidthSpinBox->setMinimum(10);
	lineWrapGuideWidthSpinBox->setMaximum(500);
	
	lineWrapGuideColorLabel = new QLabel(tr("Color"), lineWrapGuideGroupBox);
	
	lineWrapGuideColorButton = new QomposeColorPickerButton(lineWrapGuideGroupBox);
	
	lineWrapGuideLayout->addWidget( lineWrapGuideCheckBox,     0, 0, 1, 1);
	lineWrapGuideLayout->addWidget( lineWrapGuideWidthLabel,   1, 0, 1, 1);
	lineWrapGuideLayout->addWidget( lineWrapGuideWidthSpinBox, 1, 1, 1, 1);
	lineWrapGuideLayout->addWidget( lineWrapGuideColorLabel,   2, 0, 1, 1);
	lineWrapGuideLayout->addWidget( lineWrapGuideColorButton,  2, 1, 1, 1);
	
	lineWrapGuideLayout->setRowStretch(3, 1);
	lineWrapGuideLayout->setColumnStretch(0, 1);
	
	lineWrapGuideGroupBox->setLayout(lineWrapGuideLayout);
	
	// Initialize our colors group box.
	
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
	
	// Add these group boxes to our layout.
	
	layout->addWidget(generalGroupBox, 1, 0, 1, 1);
	layout->addWidget(lineWrapGuideGroupBox, 2, 0, 1, 1);
	layout->addWidget(colorsGroupBox, 3, 0, 1, 1);
	
	layout->setColumnStretch(0, 1);
	layout->setRowStretch(4, 1);
	
	setLayout(layout);
}
