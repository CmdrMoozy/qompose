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

#ifndef INCLUDE_QOMPOSE_BUFFER_H
#define INCLUDE_QOMPOSE_BUFFER_H

#include "editor/QomposeEditor.h"

#include "dialogs/QomposeFileDialog.h"

class QPrinter;

class QomposeSettings;

/*!
 * \brief This class provides high-level open/save/etc. functionality for editors.
 */
class QomposeBuffer : public QomposeEditor
{
	Q_OBJECT

	public:
		QomposeBuffer(QomposeSettings *s, QWidget *p = NULL);
		virtual ~QomposeBuffer();

		bool open(const QomposeFileDescriptor &f);
		bool revert();
		bool save(const QString &p = QString());

		QString getTitle() const;
		QString getPath() const;
		QString getDirectory() const;
		QString getFile() const;

		bool hasBeenSaved() const;
		bool isModified() const;
		void setModified(bool m);

	public Q_SLOTS:
		void print(QPrinter *p);

	private:
		QomposeSettings *settings;

		QString path;
		QString codec;

		void stripTrailingSpaces();

		bool read(bool u = false);
		bool write();

	private Q_SLOTS:
		void doModificationChanged(bool c);
		void doSettingChanged(const QString &k, const QVariant &v);

	Q_SIGNALS:
		void titleChanged(const QString &);
		void pathChanged(const QString &);
};

#endif