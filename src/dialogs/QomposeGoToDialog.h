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

#ifndef INCLUDE_QOMPOSE_GO_TO_DIALOG_H
#define INCLUDE_QOMPOSE_GO_TO_DIALOG_H

#include <QDialog>

class QGridLayout;
class QLabel;
class QLineEdit;
class QWidget;
class QPushButton;
class QShowEvent;

/*!
 * \brief This class implements a dialog to pick a line to jump to.
 */
class QomposeGoToDialog : public QDialog
{
	Q_OBJECT
	
	public:
		QomposeGoToDialog(QWidget *p = 0, Qt::WindowFlags f = 0);
		virtual ~QomposeGoToDialog();
		
		int getSelectedLine() const;
		
	protected:
		virtual void showEvent(QShowEvent *e);
		
	private:
		int selectedLine;
		
		QGridLayout *layout;
		
		QLabel *lineLabel;
		QLineEdit *lineTextEdit;
		
		QWidget *buttonsWidget;
		QGridLayout *buttonsLayout;
		QPushButton *closeButton;
		QPushButton *goToButton;
		
		void initializeGUI();
		
	private slots:
		void doGoTo();
		
	signals:
		void accepted();
};

#endif
