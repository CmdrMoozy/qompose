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

#include "Paths.h"

#include <algorithm>
#include <fstream>
#include <iterator>

#include <QFile>
#include <QFileInfo>

#include "QomposeCommon/util/Errno.h"
#include "QomposeCommon/util/ScopeExit.h"
#include "QomposeCommon/util/Strings.h"

namespace qompose
{
namespace path_utils
{
std::string getCommonParentPath(const std::vector<std::string> &paths)
{
	if(paths.size() == 0)
		return std::string();

	std::size_t maxCommonSize =
	        string_utils::getMaxCommonSize(paths.cbegin(), paths.cend());

	const char *refStart = paths.back().c_str();
	const char *refEnd = refStart + maxCommonSize;

	while(refStart != refEnd)
	{
		bool same = true;
		for(auto it = paths.cbegin(); it != paths.cend() - 1; ++it)
		{
			if(!std::equal(refStart, refEnd, it->c_str()))
			{
				same = false;
				break;
			}
		}

		if(same)
			break;

		--refEnd;
	}

	if(refStart == refEnd)
		return std::string();
	return std::string(refStart, refEnd);
}

QString getCanonicalPath(const QString &path)
{
	QFileInfo info(path);

	bool exists = info.exists();
	auto cleanupFn = [exists, path]()
	{
		if(!exists)
		{
			QFile file(path);
			file.remove();
		}
	};
	ScopeExit cleanup(cleanupFn);

	if(!exists)
		std::ofstream out(path.toStdString(), std::ios_base::out);

	return info.canonicalFilePath();
}

std::string stripSymlink(const std::string &path)
{
	QFileInfo info(QString::fromStdString(path));
	if(info.isSymLink())
		return info.symLinkTarget().toStdString();
	else
		return path;
}

std::size_t getSize(const std::string &file)
{
	QFileInfo info(QString::fromStdString(file));
	return static_cast<std::size_t>(info.size());
}
}
}