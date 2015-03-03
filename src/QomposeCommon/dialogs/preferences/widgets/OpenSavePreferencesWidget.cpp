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
/*!
 * This is our default constructor, which creates a new instance of our
 * open/save preferences widget.
 *
 * \param s The settings instance to use to save preferences values.
 * \param p The parent widget to use for this widget.
 * \param f The window flags to use for this widget.
 */
OpenSavePreferencesWidget::OpenSavePreferencesWidget(Settings *s, QWidget *p,
                                                     Qt::WindowFlags f)
        : PreferencesWidget(s, p, f),
          layout(nullptr),
          generalGroupBox(nullptr),
          generalLayout(nullptr),
          stripTrailingSpacesCheckBox(nullptr)
{
	setPreferencesIcon(gui_utils::getIconFromTheme("document-save"));
	setPreferencesTitle(tr("Open/Save"));

	initializeGUI();
}

/*!
 * This is our default destructor, which cleans up & destroys our widget.
 */
OpenSavePreferencesWidget::~OpenSavePreferencesWidget()
{
}

/*!
 * We implement our superclass's apply() function to save our various open/save
 * preferences values using our settings instance.
 */
void OpenSavePreferencesWidget::apply()
{
	// Strip Trailing Spaces

	getSettings()->setSetting(
	        "save-strip-trailing-spaces",
	        QVariant(stripTrailingSpacesCheckBox->checkState() ==
	                 Qt::Checked));
}

/*!
 * We implement our superclass's discardChanges() function to discard
 * any preferences changes that may have been made to our widget by
 * reloading the existing values from our settings instance.
 */
void OpenSavePreferencesWidget::discardChanges()
{
	// Strip Trailing Spaces

	bool stripSpaces = getSettings()
	                           ->getSetting("save-strip-trailing-spaces")
	                           .toBool();

	stripTrailingSpacesCheckBox->setCheckState(stripSpaces ? Qt::Checked
	                                                       : Qt::Unchecked);
}

/*!
 * This function initializes our widget's GUI by creating the various widgets
 * we contain, and adding them to our layout.
 */
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
