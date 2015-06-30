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

#ifndef INCLUDE_QOMPOSECOMMON_GUI_BUFFER_WIDGET_H
#define INCLUDE_QOMPOSECOMMON_GUI_BUFFER_WIDGET_H

#include <QWidget>

#include <string>
#include <vector>

#include <QByteArray>
#include <QSet>
#include <QStack>

#include "QomposeCommon/Types.h"
#include "QomposeCommon/editor/Editor.h"
#include "QomposeCommon/util/Paths.h"

class QGridLayout;
class QTabWidget;
class QPrinter;

namespace qompose
{
class Buffer;
class FindQuery;
class Pane;
class ReplaceQuery;

/*!
 * \brief This class provides high-level management of a set of buffers.
 */
class BufferWidget : public QWidget
{
	Q_OBJECT

public:
	/*!
	 * This is our default constructor, which creates a new instance of our
	 * buffers tab widget.
	 *
	 * \param p The parent widget to use for this widget.
	 */
	BufferWidget(QWidget *p = nullptr);

	BufferWidget(const BufferWidget &) = delete;
	virtual ~BufferWidget() = default;

	BufferWidget &operator=(const BufferWidget &) = delete;

	/*!
	 * This function returns the number of buffers our widget currently
	 * contains.
	 *
	 * \return The number of buffers we have.
	 */
	int count() const;

	/*!
	 * This function returns the widget at the given index in our list of
	 * widgets. If the given index is invalid, then this function will
	 * return nullptr instead.
	 *
	 * \param i The index of the desired buffer.
	 * \return The buffer at the given index.
	 */
	Buffer *bufferAt(int i) const;

	/*!
	 * This function returns our current buffer (i.e., the visible one), or
	 * nullptr if we don't have any current buffer.
	 *
	 * \return Our widget's current buffer.
	 */
	Buffer *currentBuffer() const;

	/*!
	 * This function returns whether or not our buffers widget has a valid
	 * current buffer. It will return false, for example, if all of our
	 * buffers have been closed.
	 *
	 * \return Whether or not this widget has a valid "current buffer."
	 */
	bool hasCurrentBuffer() const;

	/*!
	 * This function sets our widget's visible buffer to the buffer at the
	 * given index. If the given index is out of bounds, then no action is
	 * taken.
	 *
	 * \param i The index of the desired buffer.
	 */
	void setCurrentBuffer(int i);

	/*!
	 * This function prepares our buffers for a close event on our parent
	 * widget - for example, if we are a component of a window, and that
	 * window is about to be closed. We do this by ensuring each one of our
	 * buffers is saved (or doesn't need to be saved).
	 *
	 * We return whether or not we are totally prepared for the impending
	 * close, but our caller is in no way required to listen to us unless
	 * they are nice.
	 *
	 * \return True if we are ready to be closed, or false otherwise.
	 */
	bool prepareCloseParent();

	/*!
	 * This function attempts to find a buffer whose path matches the given
	 * value. If no such buffer can be found, then we will return -1
	 * instead of a valid tab index.
	 *
	 * \param p The path to search for.
	 * \return The index of the (first) buffer which has the given path.
	 */
	int findBufferWithPath(const QString &p);

	/*!
	 * This function returns a list of all file paths which are currently
	 * open in this buffer widget's buffers.
	 *
	 * \return The list of paths currently open.
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
	 * \return A default directory to use for open/save actions.
	 */
	QString getDefaultDirectory() const;

	/*!
	 * This function returns the deepest parent path which contains all of
	 * the files currently open in this BufferWidget. If no such path
	 * exists (because no files are open, or because they have no path in
	 * common), then an empty string will be returned instead.
	 *
	 * \return The common parent directory containing all open files.
	 */
	std::string getCommonParentPath() const;

private:
	QGridLayout *layout;
	QTabWidget *tabWidget;

	QSet<Pane *> tabs;
	QStack<ClosedBufferDescriptor> closedTabs;

	/*!
	 * This is a utility function which creates a new buffer widget,
	 * connects its signals and slots, adds it to our tab widget, and makes
	 * it the active tab.
	 *
	 * \return The newly-created buffer widget.
	 */
	Pane *newPane();

	/*!
	 * This function removes the current buffer from our tab widget, and
	 * deletes it.
	 */
	void removeCurrentBuffer();

	/*!
	 * This function moves the tab at the given index "f" to the given
	 * index "t". That is, its new index after being moved will be exactly
	 * "t." If the given from index is out-of-bounds, then no action is
	 * taken.
	 *
	 * \param f The index to move a tab from.
	 * \param t The index to move the tab to.
	 */
	void moveBuffer(int f, int t);

public Q_SLOTS:
	/*!
	 * This slot sets the encoding of the current buffer (if any). Note
	 * that this involves reloading the contents of the buffer from disk.
	 *
	 * \param e The new character encoding to use.
	 */
	void doSetEncoding(const QByteArray &e);

	/*!
	 * This slot executes a new action by creating a new buffer widget.
	 */
	void doNew();

	/*!
	 * This slot executes an open action by showing an open dialog and then
	 * opening the selected file. If we only had one "Untitled" tab before,
	 * then it will be replaced by the newly opened file.
	 */
	void doOpen();

	/*!
	 * This is a utility function which will open the file denoted by the
	 * given path directly, without displaying an "open file" dialog.
	 *
	 * \param p The path to open.
	 */
	void doOpenPath(const QString &p);

	/*!
	 * This slot executes a "reopen" action by opening the most recently
	 * closed tab in a new tab. If the list of recently closed tabs is
	 * empty, then no action is taken.
	 */
	void doReopen();

	/*!
	 * This slot executes a revert action by instructing our current buffer
	 * to revert its changes.
	 */
	void doRevert();

	/*!
	 * This slot executes a revert action on all of the buffers in our
	 * widget, by instructing each buffer to revert its changes. Note that
	 * this has no effect on buffers which have never been saved.
	 */
	void doRevertAll();

	/*!
	 * This slot executes a save action by instructing our current buffer
	 * to save its changes, potentially showing a "save as" dialog if it
	 * hasn't ever been saved before.
	 */
	void doSave();

	/*!
	 * This slot executes a "save as" action by showing a save file dialog,
	 * and then by instructing our current buffer to save to the selected
	 * path.
	 */
	void doSaveAs();

	/*!
	 * This slot executes a close action by possibly prompting to save any
	 * unsaved changes, and ultimately by removing the current buffer from
	 * our tab widget.
	 */
	void doClose();

	/*!
	 * This slot executes an undo action by instructing our current buffer
	 * to undo the most recent edit block.
	 */
	void doUndo();

	/*!
	 * This slot executes a redo action by instructing our current buffer
	 * to redo the most recent edit block.
	 */
	void doRedo();

	/*!
	 * This slot executes a cut action by instructing our current buffer to
	 * cut any selected text.
	 */
	void doCut();

	/*!
	 * This slot executes a copy action by instructing our current buffer
	 * to copy any selected text.
	 */
	void doCopy();

	/*!
	 * This slot executes a paste action by instructing our current buffer
	 * to paste any text in the clipboard.
	 */
	void doPaste();

	/*!
	 * This slot executes a "duplicate line" action by instructing our
	 * current buffer to duplicate its current line.
	 */
	void doDuplicateLine();

	/*!
	 * This slot executes a "select all" action by instructing our current
	 * buffer to select all of its contents.
	 */
	void doSelectAll();

	/*!
	 * This slot executes a deslect action by instructing our current
	 * buffer to clear any selection it currently has.
	 */
	void doDeselect();

	/*!
	 * This slot executes an "increase indent" action by instructing our
	 * current buffer to increase the indent of any selection it might
	 * have.
	 */
	void doIncreaseIndent();

	/*!
	 * This slot executes a "decrease indent" action by instructing our
	 * current buffer to decrease the indent of any selection it might
	 * have.
	 */
	void doDecreaseIndent();

	/*!
	 * This slot handles line wrapping being enabled or diabled by making
	 * the change in the current buffer.
	 *
	 * \param enabled Whether or not line wrapping is now enabled.
	 */
	void doLineWrapping(bool enabled);

	/*!
	 * This slot executes a "find next" action by instructing our current
	 * buffer to execute the given find query.
	 *
	 * \param q The find query to execute.
	 * \return The result of this find action.
	 */
	Editor::FindResult doFindNext(const FindQuery *q);

	/*!
	 * This slot executes a "find previous" action by instructing our
	 * current buffer to execute the given find query.
	 *
	 * \param q The find query to execute.
	 * \return The result of this find action.
	 */
	Editor::FindResult doFindPrevious(const FindQuery *q);

	/*!
	 * This slot executes a "replace" action by instructing our current
	 * buffer to execute the given replace query.
	 *
	 * \param q The replace query to execute.
	 * \return The result of this replace action.
	 */
	Editor::FindResult doReplace(const ReplaceQuery *q);

	/*!
	 * This slot executes a "replace in selection" action by instructing
	 * our current buffer to execute the given replace query.
	 *
	 * \param q The replace query to execute.
	 * \return The result of this "replace in selection" action.
	 */
	Editor::FindResult doReplaceSelection(const ReplaceQuery *q);

	/*!
	 * This slot executes a "replace all" action by instructing our current
	 * buffer to execute the given replace query.
	 *
	 * \param q The replace query to execute.
	 * \return The result of this "replace all" action.
	 */
	Editor::FindResult doReplaceAll(const ReplaceQuery *q);

	/*!
	 * This slot executes a "go to" action by instructing our current
	 * buffer to move its cursor to the given line.
	 *
	 * \param l The line to move our current cursor to.
	 */
	void doGoTo(int l);

	/*!
	 * This slot executes a "previous buffer" action by activating the
	 * buffer to the left of our current buffer (or the rightmost buffer,
	 * if the leftmost one is currently selected).
	 */
	void doPreviousBuffer();

	/*!
	 * This slot executes a "next buffer" action by activating the buffer
	 * to the right of our current buffer (or the leftmost buffer, if the
	 * rightmost one is currently selected).
	 */
	void doNextBuffer();

	/*!
	 * This slot executes a "move buffer left" action by moving the
	 * currently selected buffer to the left by one position. If the
	 * current buffer is already the leftmost one, then no action is taken.
	 */
	void doMoveBufferLeft();

	/*!
	 * This slot executes a "move buffer right" action by moving the
	 * currently selected buffer to the right by one position. If the
	 * current buffer is already the rightmost one, then no action is
	 * taken.
	 */
	void doMoveBufferRight();

	/*!
	 * This slot executes a "print" action by instructing our current
	 * buffer to print its contents to the given printer. If we have no
	 * current buffer, then no action is taken.
	 *
	 * \param p The printer to write the current buffer's contents to.
	 */
	void doPrint(QPrinter *p);

private Q_SLOTS:
	/*!
	 * This function handles our current tab being changed by notifying
	 * various listeners that our path has changed, and by focusing that
	 * tab's editor widget.
	 *
	 * \param i The index of the newly-active tab.
	 */
	void doTabChanged(int i);

	/*!
	 * This function handles a tab close request by clicking the "close"
	 * button on our tab bar.
	 *
	 * \param i The index of the tab being closed.
	 */
	void doTabCloseRequested(int i);

	/*!
	 * This function handles a tab closing event. More specifically, this
	 * function is called just BEFORE any tab is removed from this widget.
	 *
	 * \param i The index of the tab which is about to be closed.
	 */
	void doTabClosing(int i);

	/*!
	 * This slot handles a buffer's title being changed by updating the
	 * text on its corresponding tab. The buffer whose title changed is
	 * determined via the sender() function.
	 *
	 * \param t The new title for this buffer.
	 */
	void doTabTitleChanged(const QString &t);

	/*!
	 * This slot handles a buffer's path being changed by, if it is the
	 * current buffer, notifying our listeners that our path has changed.
	 * The buffer whose path changed is determined via the sender()
	 * function.
	 *
	 * \param p The new path for this buffer.
	 */
	void doTabPathChanged(const QString &p);

	/*!
	 * This is a utility function which will create a new buffer containing
	 * the contents of the file denoted by the given file descriptor.
	 *
	 * \param d The file descriptor to open.
	 * \return A pointer to the newly-opened buffer, or nullptr if none.
	 */
	Buffer *doOpenDescriptor(const FileDescriptor &d);

	/*!
	 * This is a utility function which attempts to reopen a closed buffer,
	 * using the given closed buffer descriptor.
	 *
	 * \param d The closed buffer descriptor to reopen.
	 */
	void doReopenBuffer(const ClosedBufferDescriptor &d);

	/*!
	 * This slot handles a buffer's encoding changing by emitting our own
	 * encodingChanged signal, if the buffer that emitted this signal is
	 * the current buffer.
	 *
	 * \param e The new encoding for the sender() buffer.
	 */
	void doBufferEncodingChanged(const QByteArray &e);

Q_SIGNALS:
	void bufferChanged();
	void pathChanged(const QString &);
	void pathOpened(const QString &);
	void encodingChanged(const QByteArray &);
	void searchWrapped();
};
}

#endif
