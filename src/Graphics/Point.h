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

#ifndef GRAPHICS_POINT_H
#define GRAPHICS_POINT_H

#include <json.hpp>

using nlohmann::json;

namespace VXHelper
{
namespace Graphics
{

class Point
{
public:
    Point() : _x(0), _y(0) {}

    Point(int x, int y) : _x(x), _y(y) {}

    int x() const;
    int y() const;

    int& rx();
    int& ry();

    void setX(int x);
    void setY(int y);

    Point& operator +=(const Point& rhs);
    Point& operator -=(const Point& rhs);
    Point& operator *=(double rhs);
    Point& operator /=(double rhs);

    bool operator ==(const Point& rhs) const;
    bool operator !=(const Point& rhs) const;

    friend Point operator +(Point lhs, const Point& rhs);
    friend Point operator -(Point lhs, const Point& rhs);
    friend Point operator *(Point lhs, double rhs);
    friend Point operator /(Point lhs, double rhs);

    Point add(const Point& rhs) const;
    Point sub(const Point& rhs) const;
    Point mul(double rhs) const;
    Point div(double rhs) const;

protected:
    int _x;
    int _y;
public:
    friend void from_json(const json& j, Point& p)
    {
        j.at("x").get_to(p._x);
        j.at("y").get_to(p._y);
    }

    friend void to_json(json& j, const Point& p)
    {
        j = json{{"x", p._x}, {"y", p._y}};
    }
};

}
}
#endif //GRAPHICS_POINT_H
