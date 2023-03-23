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

#ifndef EVENT_EVENT_H
#define EVENT_EVENT_H

#include <string>

namespace VXHelper
{
    namespace Event
    {
        class EventTarget;

        class Event
        {
            public:
                Event(const std::string& name);
                virtual ~Event();

                std::string name() const;
                void setName(const std::string& name);

                EventTarget* target() const;
                void setTarget(EventTarget* value);

                bool handled() const;
                void setHandled(bool value = true);

            protected:
                bool _handled = false;
                std::string _name;
                EventTarget* _target = nullptr;
        };
    }
}
#endif //EVENT_EVENT_H
