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

#ifndef INCLUDE_QOMPOSE_PREFERENCES_DIALOG_H
#define INCLUDE_QOMPOSE_PREFERENCES_DIALOG_H

#include <QDialog>

#include <QModelIndex>

class QGridLayout;
class QStackedWidget;
class QPushButton;

class QomposeGeneralPreferencesWidget;
class QomposeEditorPreferencesWidget;
class QomposeOpenSavePreferencesWidget;
class QomposePreferencesListView;
class QomposePreferencesListModel;
class QomposeSettings;

/*!
 * \brief This class implements a dialog containing a list of preferences widgets.
 */
class QomposePreferencesDialog : public QDialog
{
	Q_OBJECT

	public:
		QomposePreferencesDialog(QomposeSettings *s, QWidget *p = 0,
			Qt::WindowFlags f = 0);
		virtual ~QomposePreferencesDialog();

		void discardChanges();

	private:
		QomposeSettings *settings;

		QGridLayout *layout;

		QomposeGeneralPreferencesWidget *generalPreferencesWidget;
		QomposeEditorPreferencesWidget *editorPreferencesWidget;
		QomposeOpenSavePreferencesWidget *openSavePreferencesWidget;

		QomposePreferencesListView *preferencesView;
		QomposePreferencesListModel *preferencesModel;

		QStackedWidget *preferencesDisplayWidget;

		QWidget *buttonsWidget;
		QGridLayout *buttonsLayout;
		QPushButton *okButton;
		QPushButton *applyButton;
		QPushButton *cancelButton;
		QPushButton *defaultsButton;

		void createPreferencesModel();

	private Q_SLOTS:
		void doWidgetActivated(const QModelIndex &i);
		void doOk();
		void doApply();
		void doDefaults();
};

#endif
