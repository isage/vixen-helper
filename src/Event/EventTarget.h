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

#ifndef EVENT_EMITTER_H
#define EVENT_EMITTER_H

#include <memory>

#include "../Event/Handler.h"

namespace VXHelper
{
    namespace Event
    {
        class Event;
        class Dispatcher;

        class EventTarget
        {
            public:
                EventTarget(Dispatcher* eventDispatcher);
                virtual ~EventTarget();

                template <typename T>
                void emitEvent(std::unique_ptr<T> event, const Base::Delegate<T*>& handler);

            private:
                Dispatcher* _eventDispatcher;
        };
    }
}
#endif //EVENT_EMITTER_H
