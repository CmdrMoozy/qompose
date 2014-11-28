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

class Settings;
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
	PreferencesDialog(Settings *, QWidget * = nullptr,
		Qt::WindowFlags = nullptr);
	virtual ~PreferencesDialog();

	void discardChanges();

private:
	Settings *settings;

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

	PreferencesDialog(const PreferencesDialog &);
	PreferencesDialog &operator=(const PreferencesDialog &);

	void createPreferencesModel();

private Q_SLOTS:
	void doWidgetActivated(const QModelIndex &);
	void doOk();
	void doApply();
	void doDefaults();
};

}

#endif
