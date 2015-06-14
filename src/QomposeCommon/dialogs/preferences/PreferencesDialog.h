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

#ifndef INCLUDE_QOMPOSECOMMON_DIALOGS_PREFERENCES_PREFERENCES_DIALOG_H
#define INCLUDE_QOMPOSECOMMON_DIALOGS_PREFERENCES_PREFERENCES_DIALOG_H

#include <QDialog>

#include <QModelIndex>

class QGridLayout;
class QStackedWidget;
class QPushButton;

namespace qompose
{
class GeneralPreferencesWidget;
class EditorPreferencesWidget;
class OpenSavePreferencesWidget;
class PreferencesListView;
class PreferencesListModel;

/*!
 * \brief This class implements a dialog for a list of preferences widgets.
 */
class PreferencesDialog : public QDialog
{
	Q_OBJECT

public:
	/*!
	 * This is our default constructor, which creates a new instance of our
	 * preferences dialog.
	 *
	 * \param p The parent widget for this dialog.
	 * \param f The window flags for this dialog.
	 */
	PreferencesDialog(QWidget *p = nullptr, Qt::WindowFlags f = nullptr);

	PreferencesDialog(const PreferencesDialog &) = delete;
	virtual ~PreferencesDialog() = default;

	PreferencesDialog &operator=(const PreferencesDialog &) = delete;

	/*!
	 * This function discards any changes that have been made to any of our
	 * preferences widgets by instructing each widget in our model to
	 * discard its changes.
	 */
	void discardChanges();

private:
	QGridLayout *layout;

	GeneralPreferencesWidget *generalPreferencesWidget;
	EditorPreferencesWidget *editorPreferencesWidget;
	OpenSavePreferencesWidget *openSavePreferencesWidget;

	PreferencesListView *preferencesView;
	PreferencesListModel *preferencesModel;

	QStackedWidget *preferencesDisplayWidget;

	QWidget *buttonsWidget;
	QGridLayout *buttonsLayout;
	QPushButton *okButton;
	QPushButton *applyButton;
	QPushButton *cancelButton;
	QPushButton *defaultsButton;

	/*!
	 * This function initializes our preferences model by creating the
	 * model as well as all of the widgets that will be placed inside of
	 * it.
	 */
	void createPreferencesModel();

private Q_SLOTS:
	/*!
	 * This function handles a new widget being activated in our list view
	 * of preferences widgets by displaying the associated widget in our
	 * dialog's display area.
	 *
	 * \param i The model index (row) of the widget that was activated.
	 */
	void doWidgetActivated(const QModelIndex &i);

	/*!
	 * This function handles our OK button being clicked by applying all of
	 * our preferences values, and then closing our dialog.
	 */
	void doOk();

	/*!
	 * This function handles our apply button being clicked by instructing
	 * each of the preferences widgets in our model to apply their
	 * respective settings.
	 */
	void doApply();

	/*!
	 * This function handles our defaults button being clicked by resetting
	 * all of our preferences values back to their defaults, and then
	 * instructing each of the preferences widgets in our model to reload
	 * the settings from the settings instance.
	 */
	void doDefaults();
};
}

#endif
