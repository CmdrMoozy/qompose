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

#include "EditorPreferencesWidget.h"

#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QCheckBox>
#include <QSpinBox>
#include <QRadioButton>
#include <QButtonGroup>

#include "QomposeCommon/gui/ColorPickerButton.h"
#include "QomposeCommon/gui/FontPickerButton.h"
#include "QomposeCommon/gui/GUIUtils.h"
#include "QomposeCommon/util/Settings.h"

namespace qompose
{
EditorPreferencesWidget::EditorPreferencesWidget(Settings *s, QWidget *p,
                                                 Qt::WindowFlags f)
        : PreferencesWidget(s, p, f),
          layout(nullptr),
          generalGroupBox(nullptr),
          generalLayout(nullptr),
          showGutterCheckBox(nullptr),
          editorFontLabel(nullptr),
          editorFontButton(nullptr),
          indentationWidthLabel(nullptr),
          indentationWidthSpinBox(nullptr),
          indentationModeLabel(nullptr),
          indentationModeButtonGroup(nullptr),
          indentationModeTabsRadioButton(nullptr),
          indentationModeSpacesRadioButton(nullptr),
          lineWrapGuideGroupBox(nullptr),
          lineWrapGuideLayout(nullptr),
          lineWrapGuideCheckBox(nullptr),
          lineWrapGuideWidthLabel(nullptr),
          lineWrapGuideWidthSpinBox(nullptr),
          lineWrapGuideColorLabel(nullptr),
          lineWrapGuideColorButton(nullptr),
          colorsGroupBox(nullptr),
          colorsLayout(nullptr),
          editorFGLabel(nullptr),
          editorFGButton(nullptr),
          editorBGLabel(nullptr),
          editorBGButton(nullptr),
          currentLineBGLabel(nullptr),
          currentLineBGButton(nullptr),
          gutterFGLabel(nullptr),
          gutterFGButton(nullptr),
          gutterBGLabel(nullptr),
          gutterBGButton(nullptr)
{
	setPreferencesIcon(
	        gui_utils::getIconFromTheme("accessories-text-editor"));
	setPreferencesTitle(tr("Editor"));

	initializeGUI();
}

void EditorPreferencesWidget::apply()
{
	// Show Gutter

	getSettings()->setSetting(
	        "show-gutter",
	        QVariant(showGutterCheckBox->checkState() == Qt::Checked));

	// Editor Font

	getSettings()->setSetting(
	        "editor-font", QVariant(editorFontButton->getSelectedFont()));

	// Indentation Width

	getSettings()->setSetting("editor-indentation-width",
	                          QVariant(indentationWidthSpinBox->value()));

	// Indentation Mode

	getSettings()->setSetting("editor-indentation-mode",
	                          QVariant(getSelectedIndentationMode()));

	// Wrap Guide Visible

	getSettings()->setSetting(
	        "editor-wrap-guide-visible",
	        QVariant(lineWrapGuideCheckBox->checkState() == Qt::Checked));

	// Wrap Guide Width

	getSettings()->setSetting("editor-wrap-guide-width",
	                          QVariant(lineWrapGuideWidthSpinBox->value()));

	// Wrap Guide Color

	getSettings()->setSetting(
	        "editor-wrap-guide-color",
	        QVariant(lineWrapGuideColorButton->getSelectedColor()));

	// Editor Foreground

	getSettings()->setSetting("editor-foreground",
	                          QVariant(editorFGButton->getSelectedColor()));

	// Editor Background

	getSettings()->setSetting("editor-background",
	                          QVariant(editorBGButton->getSelectedColor()));

	// Current Line Background

	getSettings()->setSetting(
	        "editor-current-line",
	        QVariant(currentLineBGButton->getSelectedColor()));

	// Gutter Foreground

	getSettings()->setSetting("gutter-foreground",
	                          QVariant(gutterFGButton->getSelectedColor()));

	// Gutter Background

	getSettings()->setSetting("gutter-background",
	                          QVariant(gutterBGButton->getSelectedColor()));
}

void EditorPreferencesWidget::discardChanges()
{
	// Show Gutter

	bool showGutter = getSettings()->getSetting("show-gutter").toBool();

	showGutterCheckBox->setCheckState(showGutter ? Qt::Checked
	                                             : Qt::Unchecked);

	// Editor Font

	editorFontButton->setSelectedFont(
	        getSettings()->getSetting("editor-font").value<QFont>());

	// Indentation Width

	indentationWidthSpinBox->setValue(
	        getSettings()->getSetting("editor-indentation-width").toInt());

	// Indentation Mode

	setSelectedIndentaionMode(
	        getSettings()
	                ->getSetting("editor-indentation-mode")
	                .value<QString>());

	// Wrap Guide Visible

	bool showLineWrapGuide =
	        getSettings()->getSetting("editor-wrap-guide-visible").toBool();

	lineWrapGuideCheckBox->setCheckState(showLineWrapGuide ? Qt::Checked
	                                                       : Qt::Unchecked);

	// Wrap Guide Width

	lineWrapGuideWidthSpinBox->setValue(
	        getSettings()->getSetting("editor-wrap-guide-width").toInt());

	// Wrap Guide Color

	lineWrapGuideColorButton->setSelectedColor(
	        getSettings()
	                ->getSetting("editor-wrap-guide-color")
	                .value<QColor>());

	// Editor Foreground

	editorFGButton->setSelectedColor(
	        getSettings()->getSetting("editor-foreground").value<QColor>());

	// Editor Background

	editorBGButton->setSelectedColor(
	        getSettings()->getSetting("editor-background").value<QColor>());

	// Current Line Background

	currentLineBGButton->setSelectedColor(
	        getSettings()
	                ->getSetting("editor-current-line")
	                .value<QColor>());

	// Gutter Foreground

	gutterFGButton->setSelectedColor(
	        getSettings()->getSetting("gutter-foreground").value<QColor>());

	// Gutter Background

	gutterBGButton->setSelectedColor(
	        getSettings()->getSetting("gutter-background").value<QColor>());
}

void EditorPreferencesWidget::initializeGUI()
{
	layout = new QGridLayout(this);

	// Initialize our general group box.

	generalGroupBox = new QGroupBox(tr("General"), this);
	generalLayout = new QGridLayout(generalGroupBox);

	showGutterCheckBox = new QCheckBox(tr("Show Gutter"), generalGroupBox);

	editorFontLabel =
	        new QLabel(tr("Editor Font"), generalGroupBox, nullptr);
	editorFontButton = new FontPickerButton(generalGroupBox);

	indentationWidthLabel =
	        new QLabel(tr("Indentation Width"), generalGroupBox, nullptr);
	indentationWidthSpinBox = new QSpinBox(generalGroupBox);
	indentationWidthSpinBox->setMinimum(2);
	indentationWidthSpinBox->setMaximum(24);

	indentationModeLabel =
	        new QLabel(tr("Indentation Mode"), generalGroupBox, nullptr);
	indentationModeButtonGroup = new QButtonGroup(generalGroupBox);
	indentationModeTabsRadioButton =
	        new QRadioButton(tr("Tabs"), generalGroupBox);
	indentationModeSpacesRadioButton =
	        new QRadioButton(tr("Spaces"), generalGroupBox);
	indentationModeButtonGroup->addButton(indentationModeTabsRadioButton);
	indentationModeButtonGroup->addButton(indentationModeSpacesRadioButton);

	generalLayout->addWidget(showGutterCheckBox, 0, 0, 1, 1, nullptr);
	generalLayout->addWidget(editorFontLabel, 1, 0, 1, 1, nullptr);
	generalLayout->addWidget(editorFontButton, 1, 1, 1, 1, nullptr);
	generalLayout->addWidget(indentationWidthLabel, 2, 0, 1, 1, nullptr);
	generalLayout->addWidget(indentationWidthSpinBox, 2, 1, 1, 1, nullptr);
	generalLayout->addWidget(indentationModeLabel, 3, 0, 1, 1, nullptr);
	generalLayout->addWidget(indentationModeTabsRadioButton, 3, 1, 1, 1,
	                         nullptr);
	generalLayout->addWidget(indentationModeSpacesRadioButton, 4, 1, 1, 1,
	                         nullptr);

	generalLayout->setRowStretch(5, 1);
	generalLayout->setColumnStretch(0, 1);

	generalGroupBox->setLayout(generalLayout);

	// Initialize our line wrap guide group box.

	lineWrapGuideGroupBox = new QGroupBox(tr("Line Wrap Guide"), this);
	lineWrapGuideLayout = new QGridLayout(lineWrapGuideGroupBox);

	lineWrapGuideCheckBox =
	        new QCheckBox(tr("Show Guide (Monospaced fonts only.)"),
	                      lineWrapGuideGroupBox);

	lineWrapGuideWidthLabel = new QLabel(tr("Character Width"),
	                                     lineWrapGuideGroupBox, nullptr);

	lineWrapGuideWidthSpinBox = new QSpinBox(lineWrapGuideGroupBox);
	lineWrapGuideWidthSpinBox->setMinimum(10);
	lineWrapGuideWidthSpinBox->setMaximum(500);

	lineWrapGuideColorLabel =
	        new QLabel(tr("Color"), lineWrapGuideGroupBox, nullptr);

	lineWrapGuideColorButton = new ColorPickerButton(lineWrapGuideGroupBox);

	lineWrapGuideLayout->addWidget(lineWrapGuideCheckBox, 0, 0, 1, 1,
	                               nullptr);
	lineWrapGuideLayout->addWidget(lineWrapGuideWidthLabel, 1, 0, 1, 1,
	                               nullptr);
	lineWrapGuideLayout->addWidget(lineWrapGuideWidthSpinBox, 1, 1, 1, 1,
	                               nullptr);
	lineWrapGuideLayout->addWidget(lineWrapGuideColorLabel, 2, 0, 1, 1,
	                               nullptr);
	lineWrapGuideLayout->addWidget(lineWrapGuideColorButton, 2, 1, 1, 1,
	                               nullptr);

	lineWrapGuideLayout->setRowStretch(3, 1);
	lineWrapGuideLayout->setColumnStretch(0, 1);

	lineWrapGuideGroupBox->setLayout(lineWrapGuideLayout);

	// Initialize our colors group box.

	colorsGroupBox = new QGroupBox(tr("Colors"), this);
	colorsLayout = new QGridLayout(colorsGroupBox);

	editorFGLabel =
	        new QLabel(tr("Editor Foreground"), colorsGroupBox, nullptr);
	editorFGButton = new ColorPickerButton(colorsGroupBox);

	editorBGLabel =
	        new QLabel(tr("Editor Background"), colorsGroupBox, nullptr);
	editorBGButton = new ColorPickerButton(colorsGroupBox);

	currentLineBGLabel = new QLabel(tr("Current Line Background"),
	                                colorsGroupBox, nullptr);
	currentLineBGButton = new ColorPickerButton(colorsGroupBox);

	gutterFGLabel =
	        new QLabel(tr("Gutter Foreground"), colorsGroupBox, nullptr);
	gutterFGButton = new ColorPickerButton(colorsGroupBox);

	gutterBGLabel =
	        new QLabel(tr("Gutter Background"), colorsGroupBox, nullptr);
	gutterBGButton = new ColorPickerButton(colorsGroupBox);

	colorsLayout->addWidget(editorFGLabel, 0, 0, 1, 1, nullptr);
	colorsLayout->addWidget(editorFGButton, 0, 1, 1, 1, nullptr);
	colorsLayout->addWidget(editorBGLabel, 1, 0, 1, 1, nullptr);
	colorsLayout->addWidget(editorBGButton, 1, 1, 1, 1, nullptr);
	colorsLayout->addWidget(currentLineBGLabel, 2, 0, 1, 1, nullptr);
	colorsLayout->addWidget(currentLineBGButton, 2, 1, 1, 1, nullptr);
	colorsLayout->addWidget(gutterFGLabel, 3, 0, 1, 1, nullptr);
	colorsLayout->addWidget(gutterFGButton, 3, 1, 1, 1, nullptr);
	colorsLayout->addWidget(gutterBGLabel, 4, 0, 1, 1, nullptr);
	colorsLayout->addWidget(gutterBGButton, 4, 1, 1, 1, nullptr);

	colorsLayout->setRowStretch(5, 1);
	colorsLayout->setColumnStretch(0, 1);

	colorsGroupBox->setLayout(colorsLayout);

	// Add these group boxes to our layout.

	layout->addWidget(generalGroupBox, 1, 0, 1, 1, nullptr);
	layout->addWidget(lineWrapGuideGroupBox, 2, 0, 1, 1, nullptr);
	layout->addWidget(colorsGroupBox, 3, 0, 1, 1, nullptr);

	layout->setColumnStretch(0, 1);
	layout->setRowStretch(4, 1);

	setLayout(layout);
}

QString EditorPreferencesWidget::getSelectedIndentationMode() const
{
	if(indentationModeTabsRadioButton->isChecked())
		return "tabs";
	else if(indentationModeSpacesRadioButton->isChecked())
		return "spaces";
	else
		return "tabs";
}

void EditorPreferencesWidget::setSelectedIndentaionMode(const QString &mode)
{
	if(mode == "tabs")
		indentationModeTabsRadioButton->setChecked(true);
	else if(mode == "spaces")
		indentationModeSpacesRadioButton->setChecked(true);
	else
		indentationModeTabsRadioButton->setChecked(true);
}
}
