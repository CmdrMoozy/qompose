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
	FindDialog(QWidget * = nullptr, Qt::WindowFlags = nullptr);
	virtual ~FindDialog();

	const FindQuery *getQuery() const;

protected:
	virtual void showEvent(QShowEvent *);

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

	FindDialog(const FindDialog &);
	FindDialog &operator=(const FindDialog &);

	void initializeGUI();

private Q_SLOTS:
	void doFind();

Q_SIGNALS:
	void accepted();
};

}

#endif
