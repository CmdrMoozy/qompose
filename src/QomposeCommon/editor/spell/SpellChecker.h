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

#ifndef INCLUDE_QOMPOSECOMMON_EDITOR_SPELL_CHECKER_H
#define INCLUDE_QOMPOSECOMMON_EDITOR_SPELL_CHECKER_H

#include <QObject>

class QThread;

namespace qompose
{
class SpellCheckerWorker;

namespace editor
{
class DecoratedTextEdit;
}

/*!
 * \brief This class implements a spell checker for a decorated text edit.
 *
 * The spell checking will be performed in another thread, whenever the user
 * stops typing for a short time, and then the GUI will be updated using Qt's
 * signals and slots mechanism for cross-thread communication.
 */
class SpellChecker : public QObject
{
	Q_OBJECT

public:
	SpellChecker(editor::DecoratedTextEdit *);
	virtual ~SpellChecker();

private:
	editor::DecoratedTextEdit *editor;
	SpellCheckerWorker *worker;
	QThread *thread;

	SpellChecker(const SpellChecker &);
	SpellChecker &operator=(const SpellChecker &);

private Q_SLOTS:
	void doTextChanged();
};
}

#endif
