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

#ifndef INCLUDE_QOMPOSECOMMON_DIALOGS_REPLACE_DIALOG_H
#define INCLUDE_QOMPOSECOMMON_DIALOGS_REPLACE_DIALOG_H

#include <QDialog>

#include "QomposeCommon/editor/search/Query.h"

class QGridLayout;
class QLabel;
class QLineEdit;
class QGroupBox;
class QWidget;
class QPushButton;
class QCheckBox;
class QShowEvent;

namespace qompose
{
/*!
 * \brief This class implements a dialog to configure a replace query.
 */
class ReplaceDialog : public QDialog
{
	Q_OBJECT

public:
	/*!
	 * This is our default constructor, which creates a new instance of our
	 * replace dialog.
	 *
	 * \param p The parent widget to use for this dialog.
	 * \param f The window flags to use for this dialog.
	 */
	ReplaceDialog(QWidget *p = nullptr, Qt::WindowFlags f = nullptr);

	ReplaceDialog(const ReplaceDialog &) = delete;
	virtual ~ReplaceDialog() = default;

	ReplaceDialog &operator=(const ReplaceDialog &) = delete;

	/*!
	 * This function returns a pointer to the replace query our dialog
	 * currently has. This pointer is still good even after the dialog has
	 * been re-shown.
	 *
	 * \return The replace query containing our dialog's selected data.
	 */
	editor::search::ReplaceQuery getQuery() const;

protected:
	/*!
	 * This function handles our dialog being shown by resetting its
	 * contents using our currently selected replace query, by setting
	 * focus on the expression input box, and by raising our dialog to
	 * the front.
	 *
	 * \param e The event being handled.
	 */
	virtual void showEvent(QShowEvent *e);

private:
	editor::search::ReplaceQuery query;

	QGridLayout *layout;

	QLabel *findLabel;
	QLineEdit *findTextEdit;
	QLabel *replaceLabel;
	QLineEdit *replaceTextEdit;

	QGroupBox *optionsGroupBox;
	QGridLayout *optionsLayout;
	QCheckBox *wrapCheckBox;
	QCheckBox *wholeWordsCheckBox;
	QCheckBox *caseSensitiveCheckBox;
	QCheckBox *reverseCheckBox;
	QCheckBox *regexCheckBox;

	QWidget *buttonsWidget;
	QGridLayout *buttonsLayout;
	QPushButton *replaceButton;
	QPushButton *findButton;
	QPushButton *replaceSelButton;
	QPushButton *replaceAllButton;
	QPushButton *closeButton;

	/*!
	 * This function initializes our GUI by creating the various widgets we
	 * contain, and adding them to our layout.
	 */
	void initializeGUI();

	/*!
	 * This is a utility function which applies the currently selected
	 * options to our dialog's internal replace query.
	 */
	void applyValues();

private Q_SLOTS:
	/*!
	 * This slot handles our "replace" button being clicked by applying our
	 * dialog's options to our query, and then notifying any listeners that
	 * our replace action has been triggered.
	 */
	void doReplace();

	/*!
	 * This slot handles our "find" button being clicked by applying our
	 * dialog's options to our query, and then notifying any listeners that
	 * our find action has been triggered.
	 */
	void doFind();

	/*!
	 * This slot handles our "replace in selection" button being clicked by
	 * applying our dialog's options to our query, closing our dialog and
	 * then notifying any listeners that our "replace in selection" action
	 * has been triggered.
	 */
	void doReplaceSelection();

	/*!
	 * This slot handles our "replace all" button being clicked by applying
	 * our dialog's options to our query, closing our dialog and then
	 * notifying any listeners that our "replace all" action has been
	 * triggered.
	 */
	void doReplaceAll();

Q_SIGNALS:
	void replaceClicked();
	void findClicked();
	void replaceSelectionClicked();
	void replaceAllClicked();
};
}

#endif
