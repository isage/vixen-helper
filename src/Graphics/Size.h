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

#ifndef GRAPHICS_SIZE_H
#define GRAPHICS_SIZE_H

#include "../Graphics/Point.h"

#include <json.hpp>

using nlohmann::json;


namespace VXHelper
{
namespace Graphics
{

class Size
{
public:
    Size() : _width(0), _height(0) {}

    Size(int width, int height);

    explicit Size(const Point& rhs);

    Size& operator=(const Point& rhs);

    operator Point() const;

    int width() const;
    int height() const;

    void setWidth(int width);
    void setHeight(int height);

    Size& operator +=(const Size& rhs);
    Size& operator -=(const Size& rhs);
    Size& operator *=(double rhs);
    Size& operator /=(double rhs);

    bool operator ==(const Size& rhs) const;
    bool operator !=(const Size& rhs) const;

    friend Size operator +(Size lhs, const Size& rhs);
    friend Size operator -(Size lhs, const Size& rhs);
    friend Size operator *(Size lhs, double rhs);
    friend Size operator /(Size lhs, double rhs);

    Size add(const Size& rhs) const;
    Size sub(const Size& rhs) const;
    Size mul(double rhs) const;
    Size div(double rhs) const;

protected:
    int _width;
    int _height;
public:
    friend void from_json(const json& j, Size& p)
    {
        j.at("width").get_to(p._width);
        j.at("height").get_to(p._height);
    }

    friend void to_json(json& j, const Size& p)
    {
        j = json{{"width", p._width}, {"height", p._height}};
    }

};

}
}
#endif // GRAPHICS_SIZE_H
