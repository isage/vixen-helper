/*
        VXHelper
        Copyright (C) 2023 Cat (Ivan Epifanov)

        This program is free software: you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation, either version 3 of the License, or
        (at your option) any later version.

        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "../Event/Event.h"

namespace VXHelper
{
    namespace Event
    {
        Event::Event(const std::string& name) : _name(name)
        {
        }

        Event::~Event()
        {
        }

        std::string Event::name() const
        {
            return _name;
        }

        void Event::setName(const std::string& name)
        {
            _name = name;
        }

        EventTarget* Event::target() const
        {
            return _target;
        }

        void Event::setTarget(EventTarget* value)
        {
            _target = value;
        }

        bool Event::handled() const
        {
            return _handled;
        }

        void Event::setHandled(bool value)
        {
            _handled = value;
        }
    }
}
