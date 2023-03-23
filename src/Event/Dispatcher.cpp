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

#include "../Event/Dispatcher.h"

#include <functional>
#include <type_traits>
#include <memory>
#include <utility>

#include "../Event/EventTarget.h"
#include "../Event/Event.h"
#include "../Event/Keyboard.h"
#include "../Event/Gamepad.h"
#include "../Event/Text.h"
#include "../Event/State.h"

namespace VXHelper
{
    namespace Event
    {

        Dispatcher::AbstractTask::AbstractTask(EventTarget* target) : target(target) {}

        template <typename T>
        Dispatcher::Task<T>::Task(EventTarget* target, std::unique_ptr<T> event, Base::Delegate<T*> handler)
            : AbstractTask(target), event(std::move(event)), handler(handler)
        {
            static_assert(std::is_base_of<Event, T>::value, "T should be derived from Event::Event.");
        }

        template <typename T>
        void Dispatcher::Task<T>::perform()
        {
            event->setHandled(false);
            for (auto& func : handler.functors())
            {
                func(event.get());
                if (event->handled() || target == nullptr) break;
            }
        }

        template<typename T>
        void Dispatcher::scheduleEvent(EventTarget* target, std::unique_ptr<T> eventArg, Base::Delegate<T*> handlerArg)
        {
            _scheduledTasks.emplace_back(std::make_unique<Task<T>>(target, std::move(eventArg), std::move(handlerArg)));
        }

        void Dispatcher::processScheduledEvents()
        {
            while (!_scheduledTasks.empty())
            {
                swap(_tasksInProcess, _scheduledTasks);
                for (auto& task : _tasksInProcess)
                {
                    if (task->target == nullptr) continue;
                    task->perform();
                }
                _tasksInProcess.clear();
            }
        }

        void Dispatcher::blockEventHandlers(EventTarget* eventTarget)
        {
            _scheduledTasks.remove_if([eventTarget](std::unique_ptr<Dispatcher::AbstractTask>& task)
            {
                return (task->target == eventTarget);
            });
            for (auto& task : _tasksInProcess)
            {
                if (task->target == eventTarget)
                {
                    task->target = nullptr;
                }
            }
        }

        template void Dispatcher::scheduleEvent<Event>(EventTarget*, std::unique_ptr<Event>, Base::Delegate<Event*>);
        template void Dispatcher::scheduleEvent<Keyboard>(EventTarget*, std::unique_ptr<Keyboard>, Base::Delegate<Keyboard*>);
        template void Dispatcher::scheduleEvent<Gamepad>(EventTarget*, std::unique_ptr<Gamepad>, Base::Delegate<Gamepad*>);
        template void Dispatcher::scheduleEvent<Text>(EventTarget*, std::unique_ptr<Text>, Base::Delegate<Text*>);
        template void Dispatcher::scheduleEvent<State>(EventTarget*, std::unique_ptr<State>, Base::Delegate<State*>);
    }
}
