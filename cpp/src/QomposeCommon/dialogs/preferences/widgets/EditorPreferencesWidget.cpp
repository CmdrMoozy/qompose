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

#include <cstdint>

#include <QButtonGroup>
#include <QCheckBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QRadioButton>
#include <QSpinBox>

#include "core/config/Configuration.hpp"

#include "QomposeCommon/gui/ColorPickerButton.h"
#include "QomposeCommon/gui/FontPickerButton.h"
#include "QomposeCommon/gui/GUIUtils.h"

namespace qompose
{
EditorPreferencesWidget::EditorPreferencesWidget(QWidget *p, Qt::WindowFlags f)
        : PreferencesWidget(p, f),
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
	auto config = qompose::core::config::instance().get();
	config.set_show_gutter(showGutterCheckBox->checkState() == Qt::Checked);
	qompose::core::config::fromQFont(config.mutable_editor_font(),
	                                 editorFontButton->getSelectedFont());
	config.set_editor_indentation_width(
	        static_cast<uint64_t>(indentationWidthSpinBox->value()));
	config.set_editor_indentation_mode(getSelectedIndentationMode());
	config.set_editor_wrap_guide_visible(
	        lineWrapGuideCheckBox->checkState() == Qt::Checked);
	config.set_editor_wrap_guide_width(
	        static_cast<uint64_t>(lineWrapGuideWidthSpinBox->value()));
	qompose::core::config::fromQColor(
	        config.mutable_editor_wrap_guide_color(),
	        lineWrapGuideColorButton->getSelectedColor());
	qompose::core::config::fromQColor(config.mutable_editor_foreground(),
	                                  editorFGButton->getSelectedColor());
	qompose::core::config::fromQColor(config.mutable_editor_background(),
	                                  editorBGButton->getSelectedColor());
	qompose::core::config::fromQColor(
	        config.mutable_editor_current_line(),
	        currentLineBGButton->getSelectedColor());
	qompose::core::config::fromQColor(config.mutable_gutter_foreground(),
	                                  gutterFGButton->getSelectedColor());
	qompose::core::config::fromQColor(config.mutable_gutter_background(),
	                                  gutterBGButton->getSelectedColor());

	qompose::core::config::instance().set(config);
}

void EditorPreferencesWidget::discardChanges()
{
	auto const &config = qompose::core::config::instance().get();
	showGutterCheckBox->setCheckState(config.show_gutter() ? Qt::Checked
	                                                       : Qt::Unchecked);
	editorFontButton->setSelectedFont(
	        qompose::core::config::toQFont(config.editor_font()));
	indentationWidthSpinBox->setValue(config.editor_indentation_width());
	setSelectedIndentationMode(qompose::core::config::toString(
	        config.editor_indentation_mode()));
	lineWrapGuideCheckBox->setCheckState(config.editor_wrap_guide_visible()
	                                             ? Qt::Checked
	                                             : Qt::Unchecked);
	lineWrapGuideWidthSpinBox->setValue(config.editor_wrap_guide_width());
	lineWrapGuideColorButton->setSelectedColor(
	        qompose::core::config::toQColor(
	                config.editor_wrap_guide_color()));
	editorFGButton->setSelectedColor(
	        qompose::core::config::toQColor(config.editor_foreground()));
	editorBGButton->setSelectedColor(
	        qompose::core::config::toQColor(config.editor_background()));
	currentLineBGButton->setSelectedColor(
	        qompose::core::config::toQColor(config.editor_current_line()));
	gutterFGButton->setSelectedColor(
	        qompose::core::config::toQColor(config.gutter_foreground()));
	gutterBGButton->setSelectedColor(
	        qompose::core::config::toQColor(config.gutter_background()));
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

qompose::core::messages::Configuration::IndentationMode
EditorPreferencesWidget::getSelectedIndentationMode() const
{
	if(indentationModeTabsRadioButton->isChecked())
	{
		return qompose::core::messages::Configuration::INDENTATION_TABS;
	}
	else if(indentationModeSpacesRadioButton->isChecked())
	{
		return qompose::core::messages::Configuration::
		        INDENTATION_SPACES;
	}
	else
	{
		return qompose::core::messages::Configuration::INDENTATION_TABS;
	}
}

void EditorPreferencesWidget::setSelectedIndentationMode(
        std::string const &mode)
{
	if(mode == "tabs")
		indentationModeTabsRadioButton->setChecked(true);
	else if(mode == "spaces")
		indentationModeSpacesRadioButton->setChecked(true);
	else
		indentationModeTabsRadioButton->setChecked(true);
}
}
