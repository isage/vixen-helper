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

#ifndef EVENT_GAMEPAD_H
#define EVENT_GAMEPAD_H

#include "../Event/Event.h"

namespace VXHelper
{
    namespace Event
    {
        class Gamepad : public Event
        {
            public:
                enum class Type
                {
                    BUTTON_DOWN,
                    BUTTON_UP,
                    AXIS
                };

                static const char* typeToString(Type);

                Gamepad(Type type);
                Gamepad(const Gamepad& event, const std::string& newName);
                Gamepad(const Gamepad& event);
                ~Gamepad() override;

                Type originalType() const;

                int button() const;
                void setButton(int value);

                int axis() const;
                void setAxis(int value);
                int axisValue() const;
                void setAxisValue(int value);

                int gamepad() const;
                void setGamepad(int value);

            protected:
                int _gamepad = 0;
                int _button = 0;
                int _axis = 0;
                int _axisvalue = 0;

                Type _type;
        };
    }
}
#endif // EVENT_GAMEPAD_H
