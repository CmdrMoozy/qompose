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

#include "OpenSavePreferencesWidget.h"

#include <QGridLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QVariant>

#include "QomposeCommon/gui/GUIUtils.h"
#include "QomposeCommon/util/Settings.h"

namespace qompose
{
OpenSavePreferencesWidget::OpenSavePreferencesWidget(QWidget *p,
                                                     Qt::WindowFlags f)
        : PreferencesWidget(p, f),
          layout(nullptr),
          generalGroupBox(nullptr),
          generalLayout(nullptr),
          stripTrailingSpacesCheckBox(nullptr)
{
	setPreferencesIcon(gui_utils::getIconFromTheme("document-save"));
	setPreferencesTitle(tr("Open/Save"));

	initializeGUI();
}

void OpenSavePreferencesWidget::apply()
{
	// Strip Trailing Spaces

	Settings::instance().setSetting(
	        "save-strip-trailing-spaces",
	        QVariant(stripTrailingSpacesCheckBox->checkState() ==
	                 Qt::Checked));
}

void OpenSavePreferencesWidget::discardChanges()
{
	// Strip Trailing Spaces

	bool stripSpaces = Settings::instance()
	                           .getSetting("save-strip-trailing-spaces")
	                           .toBool();

	stripTrailingSpacesCheckBox->setCheckState(stripSpaces ? Qt::Checked
	                                                       : Qt::Unchecked);
}

void OpenSavePreferencesWidget::initializeGUI()
{
	layout = new QGridLayout(this);

	// Initialize our general group box.

	generalGroupBox = new QGroupBox(tr("General"), this);
	generalLayout = new QGridLayout(generalGroupBox);

	stripTrailingSpacesCheckBox =
	        new QCheckBox(tr("Strip Trailing Spaces"), generalGroupBox);

	generalLayout->addWidget(stripTrailingSpacesCheckBox, 0, 0, 1, 1,
	                         nullptr);

	generalLayout->setRowStretch(1, 1);
	generalLayout->setColumnStretch(0, 1);

	generalGroupBox->setLayout(generalLayout);

	// Add these group boxes to our layout.

	layout->addWidget(generalGroupBox, 0, 0, 1, 1, nullptr);

	layout->setRowStretch(1, 1);
	layout->setColumnStretch(0, 1);

	setLayout(layout);
}
}
