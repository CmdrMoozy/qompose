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

#include "GeneralPreferencesWidget.h"

#include <QGridLayout>
#include <QCheckBox>
#include <QLabel>
#include <QSpinBox>

#include "QomposeCommon/gui/GUIUtils.h"
#include "QomposeCommon/util/Settings.h"

namespace qompose
{

/*!
 * This is our default constructor, which creates a new instance of our general
 * preferences widget.
 *
 * \param s The settings instance to use to persist settings values.
 * \param p The parent widget to use for this widget.
 * \param f The window flags to use for this widget.
 */
GeneralPreferencesWidget::GeneralPreferencesWidget(Settings *s,
		QWidget *p, Qt::WindowFlags f)
	: PreferencesWidget(s, p, f), layout(nullptr),
		statusBarCheckBox(nullptr), recentListSizeLabel(nullptr),
		recentListSizeSpinBox(nullptr),
		saveWindowAttribsCheckBox(nullptr)
{
	setPreferencesIcon(GUIUtils::getIconFromTheme("preferences-other"));
	setPreferencesTitle(tr("General"));

	initializeGUI();
}

/*!
 * This is our default destructor, which cleans up & destroys our widget.
 */
GeneralPreferencesWidget::~GeneralPreferencesWidget()
{
}

/*!
 * We implement our superclass's apply() function to save our various general
 * preferences values using our settings instance.
 */
void GeneralPreferencesWidget::apply()
{
	// Show Status Bar

	getSettings()->setSetting("show-status-bar", QVariant(
		statusBarCheckBox->checkState() == Qt::Checked));

	// Recently Opened List Size

	getSettings()->setSetting("recent-list-size", QVariant(
		recentListSizeSpinBox->value()));

	// Save Window Attributes

	getSettings()->setSetting("window-save-attributes", QVariant(
		saveWindowAttribsCheckBox->checkState() == Qt::Checked));
}

/*!
 * We implement our superclass's discardChanges() function to discard
 * any preferences changes that may have been made to our widget by
 * reloading the existing values from our settings instance.
 */
void GeneralPreferencesWidget::discardChanges()
{
	// Show Status Bar

	bool showStatusBar = getSettings()->getSetting(
		"show-status-bar").toBool();

	statusBarCheckBox->setCheckState(showStatusBar ?
		Qt::Checked : Qt::Unchecked);

	// Recently Opened List Size

	recentListSizeSpinBox->setValue(getSettings()->getSetting(
		"recent-list-size").toInt());

	// Save Window Attributes

	bool saveWindowAttribs = getSettings()->getSetting(
		"window-save-attributes").toBool();

	saveWindowAttribsCheckBox->setCheckState(saveWindowAttribs ?
		Qt::Checked : Qt::Unchecked);
}

/*!
 * This function initializes our widget's GUI by creating the various widgets
 * we contain, and adding them to our layout.
 */
void GeneralPreferencesWidget::initializeGUI()
{
	layout = new QGridLayout(this);

	statusBarCheckBox = new QCheckBox(tr("Show Status Bar"), this);

	recentListSizeLabel = new QLabel(
		tr("Recently Opened List Size"), this, nullptr);

	recentListSizeSpinBox = new QSpinBox(this);
	recentListSizeSpinBox->setMinimum(0);
	recentListSizeSpinBox->setMaximum(50);

	saveWindowAttribsCheckBox = new QCheckBox(tr("Save Window Attributes on Exit"), this);

	layout->addWidget(statusBarCheckBox, 0, 0, 1, 1, nullptr);
	layout->addWidget(recentListSizeLabel, 1, 0, 1, 1, nullptr);
	layout->addWidget(recentListSizeSpinBox, 1, 1, 1, 1, nullptr);
	layout->addWidget(saveWindowAttribsCheckBox, 2, 0, 1, 1, nullptr);

	layout->setColumnStretch(0, 1);
	layout->setRowStretch(3, 1);

	setLayout(layout);
}

}
