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

#ifndef EVENT_TEXT_H
#define EVENT_TEXT_H

#include "../Event/Event.h"

namespace VXHelper
{
    namespace Event
    {
        class Text : public Event
        {
            public:
                enum class Type
                {
                    INPUT,
                    EDITING
                };

                static const char* typeToString(Type);

                Text(Type type);
                Text(const Text& event, const std::string& newName);
                Text(const Text& event);
                ~Text() override;

                Type originalType() const;

                std::string& text();
                void setText(std::string text);
                void setText(char* text);


            protected:

                Type _type;
                std::string _text;
        };
    }
}
#endif // EVENT_TEXT_H
