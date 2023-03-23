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

#include "../Event/EventTarget.h"

#include <type_traits>

#include "../Event/Dispatcher.h"
#include "../Event/Event.h"
#include "../Event/Keyboard.h"
#include "../Event/Gamepad.h"
#include "../Event/Text.h"
#include "../Event/State.h"

namespace VXHelper
{
    namespace Event
    {
        EventTarget::EventTarget(Dispatcher* eventDispatcher) : _eventDispatcher(eventDispatcher)
        {
        }

        EventTarget::~EventTarget()
        {
            _eventDispatcher->blockEventHandlers(this);
        }

        template<typename T>
        void EventTarget::emitEvent(std::unique_ptr<T> event, const Base::Delegate<T*>& handler)
        {
            static_assert(std::is_base_of<Event, T>::value, "T should be derived from Event::Event.");
            if (handler)
            {
                event->setTarget(this);
                _eventDispatcher->scheduleEvent<T>(this, std::move(event), handler);
            }
        }

        template void EventTarget::emitEvent<Event>(std::unique_ptr<Event>, const Base::Delegate<Event*>&);
        template void EventTarget::emitEvent<Keyboard>(std::unique_ptr<Keyboard>, const Base::Delegate<Keyboard*>&);
        template void EventTarget::emitEvent<Gamepad>(std::unique_ptr<Gamepad>, const Base::Delegate<Gamepad*>&);
        template void EventTarget::emitEvent<Text>(std::unique_ptr<Text>, const Base::Delegate<Text*>&);
        template void EventTarget::emitEvent<State>(std::unique_ptr<State>, const Base::Delegate<State*>&);
    }
}
