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

#ifndef INCLUDE_QOMPOSE_ABOUT_DIALOG_H
#define INCLUDE_QOMPOSE_ABOUT_DIALOG_H

#include <QDialog>

class QGridLayout;
class QTabWidget;
class QTextEdit;
class QPushButton;

class QomposeAboutDialog : public QDialog
{
	Q_OBJECT
	
	public:
		QomposeAboutDialog(QWidget *p = 0, Qt::WindowFlags f = 0);
		virtual ~QomposeAboutDialog();
		
	private:
		QGridLayout *layout;
		QTabWidget *tabs;
		
		QTextEdit *licenseTextEdit;
		
		QPushButton *closeButton;
		
		void loadLicense();
		
	private slots:
		void doClose();
};

#endif