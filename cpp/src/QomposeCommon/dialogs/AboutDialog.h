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

#ifndef INCLUDE_QOMPOSECOMMON_DIALOGS_ABOUT_DIALOG_H
#define INCLUDE_QOMPOSECOMMON_DIALOGS_ABOUT_DIALOG_H

#include <QDialog>

class QGridLayout;
class QTabWidget;
class QTextEdit;
class QPushButton;
class QWidget;
class QLabel;

namespace qompose
{
/*!
 * \brief This class implements our "About" dialog.
 */
class AboutDialog : public QDialog
{
public:
	/*!
	 * This is our default constructor, which creates a new about dialog
	 * instance and loads the contents from our resources into its UI.
	 *
	 * \param p This dialog's parent widget.
	 * \param f The window flags to use for this dialog.
	 */
	AboutDialog(QWidget *p = nullptr, Qt::WindowFlags f = nullptr);

	AboutDialog(const AboutDialog &) = delete;

	virtual ~AboutDialog();

	AboutDialog &operator=(const AboutDialog &) = delete;

private:
	QGridLayout *layout;
	QTabWidget *tabs;

	QTextEdit *licenseTextEdit;

	QWidget *qomposeTab;
	QGridLayout *qomposeTabLayout;
	QLabel *qomposeIconLabel;
	QLabel *qomposeLabel;
	QLabel *qomposeDescriptionLabel;
	QTextEdit *qomposeTextEdit;

	QPushButton *closeButton;

	/*!
	 * This is a utility function which initializes our GUI elements and
	 * adds them to our layout.
	 */
	void initializeGUI();

	/*!
	 * This is a utility function which loads the body of our license file,
	 * and places its contents in our license text edit.
	 */
	void loadLicense();
};
}

#endif
