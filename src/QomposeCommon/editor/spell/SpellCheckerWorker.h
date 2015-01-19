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

#ifndef INCLUDE_QOMPOSECOMMON_EDITOR_SPELL_CHECKER_WORKER_H
#define INCLUDE_QOMPOSECOMMON_EDITOR_SPELL_CHECKER_WORKER_H

#include <QObject>

class QTimer;

namespace qompose
{
/*!
 * \brief This class implements the worker object for our spell checker.
 *
 * This class generally shouldn't be used by itself; SpellChecker uses this
 * class in a non-GUI thread to perform the work, and then connects it properly
 * so the GUI can be updated once it does its job.
 */
class SpellCheckerWorker : public QObject
{
	Q_OBJECT

public:
	SpellCheckerWorker(QObject * = nullptr);
	virtual ~SpellCheckerWorker();

private:
	QTimer *timer;

	SpellCheckerWorker(const SpellCheckerWorker &);
	SpellCheckerWorker &operator=(const SpellCheckerWorker &);

private Q_SLOTS:
	void doSpellCheck();
};
}

#endif
