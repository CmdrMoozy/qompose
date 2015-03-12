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

#ifndef INCLUDE_QOMPOSECOMMON_DIALOGS_FIND_DIALOG_H
#define INCLUDE_QOMPOSECOMMON_DIALOGS_FIND_DIALOG_H

#include <QDialog>

class QGridLayout;
class QLabel;
class QLineEdit;
class QGroupBox;
class QCheckBox;
class QWidget;
class QPushButton;
class QShowEvent;

namespace qompose
{
class FindQuery;

/*!
 * \brief This class implements a dialog to configure a find query.
 */
class FindDialog : public QDialog
{
	Q_OBJECT

public:
	/*!
	 * This is our default constructor, which creates a new instance of our
	 * find dialog.
	 *
	 * \param p The parent widget to use for this dialog.
	 * \param f The window flags to use for this dialog.
	 */
	FindDialog(QWidget *p = nullptr, Qt::WindowFlags f = nullptr);

	FindDialog(const FindDialog &) = delete;
	virtual ~FindDialog() = default;

	FindDialog &operator=(const FindDialog &) = delete;

	/*!
	 * This function returns a pointer to the find query our dialog
	 * currently has. This pointer is still good even after the dialog has
	 * been re-shown.
	 *
	 * \return The find query containing our dialog's selected data.
	 */
	const FindQuery *getQuery() const;

protected:
	/*!
	 * This function handles our dialog being shown by resetting its
	 * contents using our currently selected find query, by setting
	 * focus on the expression input box, and by raising our dialog to
	 * the front.
	 *
	 * \param e The event being handled.
	 */
	virtual void showEvent(QShowEvent *e);

private:
	FindQuery *query;

	QGridLayout *layout;

	QLabel *findLabel;
	QLineEdit *findTextEdit;

	QGroupBox *optionsGroupBox;
	QGridLayout *optionsLayout;
	QCheckBox *wrapCheckBox;
	QCheckBox *wholeWordsCheckBox;
	QCheckBox *caseSensitiveCheckBox;
	QCheckBox *reverseCheckBox;
	QCheckBox *regexCheckBox;

	QWidget *buttonsWidget;
	QGridLayout *buttonsLayout;
	QPushButton *findButton;
	QPushButton *closeButton;

	/*!
	 * This function initializes our GUI by creating the various widgets we
	 * contain, and adding them to our layout.
	 */
	void initializeGUI();

private Q_SLOTS:
	/*!
	 * This function handles our "find" button being clicked by applying
	 * our dialog's contents to our find query object, and by alerting our
	 * callers that our dialog has been accepted.
	 */
	void doFind();

Q_SIGNALS:
	void accepted();
};
}

#endif
