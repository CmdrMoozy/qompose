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

class ReplaceQuery;

/*!
 * \brief This class implements a dialog to configure a replace query.
 */
class ReplaceDialog : public QDialog
{
	Q_OBJECT

public:
	ReplaceDialog(QWidget * = nullptr, Qt::WindowFlags = nullptr);
	virtual ~ReplaceDialog();

	const ReplaceQuery *getQuery() const;

protected:
	virtual void showEvent(QShowEvent *);

private:
	ReplaceQuery *query;

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

	ReplaceDialog(const ReplaceDialog &);
	ReplaceDialog &operator=(const ReplaceDialog &);

	void initializeGUI();

	void applyValues();

private Q_SLOTS:
	void doReplace();
	void doFind();
	void doReplaceSelection();
	void doReplaceAll();

Q_SIGNALS:
	void replaceClicked();
	void findClicked();
	void replaceSelectionClicked();
	void replaceAllClicked();
};

}

#endif
