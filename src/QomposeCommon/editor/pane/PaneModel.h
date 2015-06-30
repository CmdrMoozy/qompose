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

#ifndef INCLUDE_QOMPOSECOMMON_EDITOR_PANE_PANE_MODEL_H
#define INCLUDE_QOMPOSECOMMON_EDITOR_PANE_PANE_MODEL_H

#include <cstddef>
#include <string>
#include <vector>

#include <QObject>
#include <QString>

namespace qompose
{
class Pane;

class PaneModel : public QObject
{
	Q_OBJECT

public:
	PaneModel(QObject *p = nullptr);

	PaneModel(const PaneModel &) = delete;
	virtual ~PaneModel() = default;
	PaneModel &operator=(const PaneModel &) = delete;

	/*!
	 * \return The number of panes in this model.
	 */
	std::size_t count() const;

	/*!
	 * This function returns the pane at the given index in this model.
	 * If the given index is invalid, then this function will return
	 * nullptr instead.
	 *
	 * \param i The index of the desired pane.
	 * \return The pane at the given index.
	 */
	Pane *paneAt(std::size_t i) const;

	/*!
	 * This function attempts to find a pane whose path matches the given
	 * value. If no such pane can be found, then nullptr is returned
	 * instead.
	 *
	 * \param p The path to search for.
	 * \return The (first) pane which has the given path.
	 */
	Pane *findPaneWithPath(const QString &p) const;

	/*!
	 * \return The list of all currently open paths in this model.
	 */
	std::vector<std::string> getOpenPaths() const;

	/*!
	 * This function tries to compute a default directory for, e.g., open
	 * or save actions, based upon our current buffer list.
	 *
	 * We will use the first valid path of the current buffer's path, the
	 * path of the nearest buffer to the left, the path of the nearest
	 * buffer to the right, or the user's home directory.
	 *
	 * \param current The "current" pane.
	 * \return A default directory to use for open/save actions.
	 */
	QString getDefaultDirectory(Pane *current = nullptr) const;

	/*!
	 * This function returns the deepest parent path which contains all of
	 * the files currently open in this model. If no such path exists
	 * (because no files are open, or because they have no path in common),
	 * then an empty string will be returned instead.
	 *
	 * \return The common parent directory containing all open files.
	 */
	QString getCommonParentPath() const;

	/*!
	 * This function will move the pane at the given "from" index to be at
	 * the given "to" index in the model. No signal is emitted by this
	 * function; it is assumed that the view is already aware of this
	 * change, and has accounted for it correctly.
	 *
	 * The behavior is undefined if either index is invalid (both indexes
	 * must be in the range 0 <= index < count()).
	 *
	 * \param f The index of the Pane to move.
	 * \param t The new index for the specified pane.
	 */
	void movePane(std::size_t f, std::size_t t);

	Pane *newPane();

	void removePane(std::size_t i);
	void removePane(const Pane *p);

private:
	std::vector<Pane *> panes;

Q_SIGNALS:
	void paneAdded(Pane *, std::size_t);
	void paneRemoved(std::size_t);
};
}

#endif
