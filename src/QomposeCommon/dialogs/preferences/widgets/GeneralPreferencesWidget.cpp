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

#include <cstdint>

#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QSpinBox>

#include "core/config/Configuration.hpp"

#include "QomposeCommon/gui/GUIUtils.h"

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
	auto config = qompose::core::config::instance().get();
	config.set_show_file_in_title(showFileInTitleCheckBox->checkState() ==
	                              Qt::Checked);
	config.set_show_status_bar(statusBarCheckBox->checkState() ==
	                           Qt::Checked);
	config.set_recent_list_size_max(
	        static_cast<uint64_t>(recentListSizeSpinBox->value()));
	config.set_window_save_attributes(
	        saveWindowAttribsCheckBox->checkState() == Qt::Checked);
	qompose::core::config::instance().set(config);
}

void GeneralPreferencesWidget::discardChanges()
{
	auto const &config = qompose::core::config::instance().get();
	showFileInTitleCheckBox->setCheckState(
	        config.show_file_in_title() ? Qt::Checked : Qt::Unchecked);
	statusBarCheckBox->setCheckState(
	        config.show_status_bar() ? Qt::Checked : Qt::Unchecked);
	recentListSizeSpinBox->setValue(config.recent_list_size_max());
	saveWindowAttribsCheckBox->setCheckState(
	        config.window_save_attributes() ? Qt::Checked : Qt::Unchecked);
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
