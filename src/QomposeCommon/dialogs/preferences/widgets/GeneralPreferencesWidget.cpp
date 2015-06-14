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
GeneralPreferencesWidget::GeneralPreferencesWidget(QWidget *p,
                                                   Qt::WindowFlags f)
        : PreferencesWidget(p, f),
          layout(nullptr),
          showFileInTitleCheckBox(nullptr),
          statusBarCheckBox(nullptr),
          recentListSizeLabel(nullptr),
          recentListSizeSpinBox(nullptr),
          saveWindowAttribsCheckBox(nullptr)
{
	setPreferencesIcon(gui_utils::getIconFromTheme("preferences-other"));
	setPreferencesTitle(tr("General"));

	initializeGUI();
}

void GeneralPreferencesWidget::apply()
{
	// Show File In Title

	Settings::instance().setSetting(
	        "show-file-in-title",
	        QVariant(showFileInTitleCheckBox->checkState() == Qt::Checked));

	// Show Status Bar

	Settings::instance().setSetting(
	        "show-status-bar",
	        QVariant(statusBarCheckBox->checkState() == Qt::Checked));

	// Recently Opened List Size

	Settings::instance().setSetting(
	        "recent-list-size", QVariant(recentListSizeSpinBox->value()));

	// Save Window Attributes

	Settings::instance().setSetting(
	        "window-save-attributes",
	        QVariant(saveWindowAttribsCheckBox->checkState() ==
	                 Qt::Checked));
}

void GeneralPreferencesWidget::discardChanges()
{
	// Show File In Title

	bool showFileInTitle =
	        Settings::instance().getSetting("show-file-in-title").toBool();

	showFileInTitleCheckBox->setCheckState(showFileInTitle ? Qt::Checked
	                                                       : Qt::Unchecked);

	// Show Status Bar

	bool showStatusBar =
	        Settings::instance().getSetting("show-status-bar").toBool();

	statusBarCheckBox->setCheckState(showStatusBar ? Qt::Checked
	                                               : Qt::Unchecked);

	// Recently Opened List Size

	recentListSizeSpinBox->setValue(
	        Settings::instance().getSetting("recent-list-size").toInt());

	// Save Window Attributes

	bool saveWindowAttribs = Settings::instance()
	                                 .getSetting("window-save-attributes")
	                                 .toBool();

	saveWindowAttribsCheckBox->setCheckState(
	        saveWindowAttribs ? Qt::Checked : Qt::Unchecked);
}

void GeneralPreferencesWidget::initializeGUI()
{
	layout = new QGridLayout(this);

	showFileInTitleCheckBox =
	        new QCheckBox(tr("Show Filename in Window Title"), this);

	statusBarCheckBox = new QCheckBox(tr("Show Status Bar"), this);

	recentListSizeLabel =
	        new QLabel(tr("Recently Opened List Size"), this, nullptr);

	recentListSizeSpinBox = new QSpinBox(this);
	recentListSizeSpinBox->setMinimum(0);
	recentListSizeSpinBox->setMaximum(50);

	saveWindowAttribsCheckBox =
	        new QCheckBox(tr("Save Window Attributes on Exit"), this);

	layout->addWidget(showFileInTitleCheckBox, 0, 0, 1, 1, nullptr);
	layout->addWidget(statusBarCheckBox, 1, 0, 1, 1, nullptr);
	layout->addWidget(recentListSizeLabel, 2, 0, 1, 1, nullptr);
	layout->addWidget(recentListSizeSpinBox, 2, 1, 1, 1, nullptr);
	layout->addWidget(saveWindowAttribsCheckBox, 3, 0, 1, 1, nullptr);

	layout->setColumnStretch(0, 1);
	layout->setRowStretch(4, 1);

	setLayout(layout);
}
}
