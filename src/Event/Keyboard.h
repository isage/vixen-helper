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

#ifndef EVENT_KEYBOARD_H
#define EVENT_KEYBOARD_H

#include "../Event/Event.h"

namespace VXHelper
{
    namespace Event
    {
        class Keyboard : public Event
        {
            public:
                enum class Type
                {
                    KEY_DOWN,
                    KEY_UP
                };

                static const char* typeToString(Type);

                Keyboard(Type type);
                Keyboard(const Keyboard& event, const std::string& newName);
                Keyboard(const Keyboard& event);
                ~Keyboard() override;

                Type originalType() const;

                int keyCode() const;
                void setKeyCode(int value);

                bool shiftPressed() const;
                void setShiftPressed(bool value);

                bool controlPressed() const;
                void setControlPressed(bool value);

                bool altPressed() const;
                void setAltPressed(bool value);

            protected:
                bool _altPressed = false;
                bool _controlPressed = false;
                bool _shiftPressed = false;
                int _keyCode = 0;

                Type _type;
        };
    }
}
#endif // EVENT_KEYBOARD_H
