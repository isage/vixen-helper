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

#include "../Event/Text.h"
#include "../Utils/Logger.h"

namespace VXHelper
{
    namespace Event
    {
        Text::Text(Text::Type type) : Event(typeToString(type))
        {
            _type = type;
        }

        Text::Text(const Text& event, const std::string& newName) : Event(newName)
        {
            _text = event._text;
        }

        Text::Text(const Text& event) : Text(event, event._name)
        {
        }

        Text::~Text()
        {
        }

        const char* Text::typeToString(Text::Type type)
        {
            switch (type)
            {
                case Type::EDITING: return "input";
                case Type::INPUT:   return "editing";
                default: return "Text";
            }
        }

        Text::Type Text::originalType() const
        {
            return _type;
        }

        void Text::setText(std::string text)
        {
            _text = text;
        }

        void Text::setText(char* text)
        {
            _text = std::string(text);
        }

        std::string& Text::text()
        {
            return _text;
        }
    }
}
