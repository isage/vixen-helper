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

#include "../Event/Gamepad.h"

namespace VXHelper
{
    namespace Event
    {
        Gamepad::Gamepad(Gamepad::Type type) : Event(typeToString(type))
        {
            _type = type;
        }

        Gamepad::Gamepad(const Gamepad& event, const std::string& newName) : Event(newName)
        {
            _button = event._button;
        }

        Gamepad::Gamepad(const Gamepad& event) : Gamepad(event, event._name)
        {
        }

        Gamepad::~Gamepad()
        {
        }

        const char* Gamepad::typeToString(Gamepad::Type type)
        {
            switch (type)
            {
                case Type::BUTTON_DOWN: return "buttdown";
                case Type::BUTTON_UP:   return "buttup";
                case Type::AXIS:        return "axis";
                default: return "Gamepad";
            }
        }

        Gamepad::Type Gamepad::originalType() const
        {
            return _type;
        }

        int Gamepad::button() const
        {
            return _button;
        }

        void Gamepad::setButton(int value)
        {
            _button = value;
        }

        int Gamepad::axis() const
        {
            return _axis;
        }

        void Gamepad::setAxis(int value)
        {
            _axis = value;
        }

        int Gamepad::axisValue() const
        {
            return _axisvalue;
        }

        void Gamepad::setAxisValue(int value)
        {
            _axisvalue = value;
        }

        int Gamepad::gamepad() const
        {
            return _gamepad;
        }

        void Gamepad::setGamepad(int value)
        {
            _gamepad = value;
        }

    }
}
