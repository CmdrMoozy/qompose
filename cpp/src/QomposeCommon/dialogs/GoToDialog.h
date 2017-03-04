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

#ifndef INCLUDE_QOMPOSECOMMON_DIALOGS_GO_TO_DIALOG_H
#define INCLUDE_QOMPOSECOMMON_DIALOGS_GO_TO_DIALOG_H

#include <QDialog>

class QGridLayout;
class QLabel;
class QLineEdit;
class QWidget;
class QPushButton;
class QShowEvent;

namespace qompose
{
/*!
 * \brief This class implements a dialog to pick a line to jump to.
 */
class GoToDialog : public QDialog
{
	Q_OBJECT

public:
	/*!
	 * This is our default constructor, which initializes a new instance of
	 * our "go to" dialog.
	 *
	 * \param p The parent widget for our dialog.
	 * \param f The window falgs to use for our dialog.
	 */
	GoToDialog(QWidget *p = nullptr, Qt::WindowFlags f = nullptr);

	GoToDialog(const GoToDialog &) = delete;
	virtual ~GoToDialog() = default;

	GoToDialog &operator=(const GoToDialog &) = delete;

	/*!
	 * This function returns the currently selected line. This value is 0
	 * by default, and is only updated when our "Go To" button is clicked.
	 *
	 * \return Our dialog's currently selected line.
	 */
	int getSelectedLine() const;

protected:
	/*!
	 * This function handles our dialog being shown by resetting our
	 * dialog's contents, setting focus on the correct dialog elements, and
	 * by raising our dialog to the front.
	 *
	 * \param e The event being handled.
	 */
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

	/*!
	 * This function initializes our dialog's GUI by creating our various
	 * widgets and adding them to our layout.
	 */
	void initializeGUI();

private Q_SLOTS:
	/*!
	 * This function performs our "Go To" operation by updating our
	 * selected line, and then emitting a signal letting our callers know
	 * that our dialog has been accepted.
	 *
	 * If the currently selected line number is invalid (i.e., isn't an
	 * integer), then we will show an error instead, and avoid closing the
	 * dialog.
	 */
	void doGoTo();

Q_SIGNALS:
	void accepted();
};
}

#endif
