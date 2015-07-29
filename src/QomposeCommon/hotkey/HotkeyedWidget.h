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

#ifndef INCLUDE_QOMPOSECOMMON_HOTKEY_HOTKEYED_WIDGET_H
#define INCLUDE_QOMPOSECOMMON_HOTKEY_HOTKEYED_WIDGET_H

#include <functional>
#include <utility>

#include <QKeyEvent>

#include "QomposeCommon/hotkey/HotkeyMap.h"

namespace qompose
{
namespace hotkey
{
/*!
 * \brief Provides hotkey support for an arbitrary Qt widget type.
 *
 * This class implements all of the common code needed to support hotkeys by
 * intercepting keyPressEvent() for an arbitrary Qt widget type.
 */
template <typename T> class HotkeyedWidget : public T
{
public:
	template <typename... Arg>
	HotkeyedWidget(Arg... arg)
	        : T(std::forward<Arg>(arg)...), hotkeys()
	{
	}

	virtual ~HotkeyedWidget() = default;

protected:
	virtual void keyPressEvent(QKeyEvent *e) override
	{
		const std::function<void()> *handler =
		        hotkeys.getHotkeyHandler(e);

		if(handler != nullptr)
			(*handler)();
		else
			T::keyPressEvent(e);
	}

	bool addHotkey(Hotkey const &h, std::function<void()> const &f)
	{
		return hotkeys.addHotkey(h, f);
	}

private:
	HotkeyMap hotkeys;
};
}
}

#endif
