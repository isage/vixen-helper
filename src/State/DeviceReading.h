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

#ifndef STATE_DEVICE_READING_H
#define STATE_DEVICE_READING_H

#include "../State/State.h"
#if defined(__VITA__)
#include <psp2common/types.h>
#endif

namespace VXHelper
{
    namespace State
    {
        class DeviceReading : public State
        {
            public:
                DeviceReading(uint16_t vid, uint16_t pid);
                ~DeviceReading() override;

                void think(uint32_t delta) override;
                void init() override;
                void render() override;
                void onKeyDown(Event::Keyboard* event) override;
                void onButtonDown(Event::Gamepad* event) override;
            private:
                int _state = 0;
                uint16_t _vid;
                uint16_t _pid;
#if defined(__VITA__)
                SceUID _mod_id;
#endif
        };
    }
}
#endif // STATE_DEVICE_READING_H
