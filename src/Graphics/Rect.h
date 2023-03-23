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

#ifndef GRAPHICS_RECT_H
#define GRAPHICS_RECT_H

#include "../Graphics/Point.h"
#include "../Graphics/Size.h"

#include <json.hpp>

using nlohmann::json;

namespace VXHelper
{
namespace Graphics
{

class Rect
{
public:
    Rect() : _topLeft(0, 0), _size(0, 0) {}

    Rect(int x, int y, int width, int height);

    int x() const;
    int y() const;

    void setX(int x);
    void setY(int y);

    int width() const;
    int height() const;

    void setWidth(int width);
    void setHeight(int height);


    static bool inRect(const Point& needle, const Size& size);

    static bool inRect(const Point& needle, const Point& topLeft, const Size& size);

    static bool intersects(const Point& topLeft1, const Size& size1, const Point& topLeft2, const Size& size2);
private:
    Point _topLeft;
    Size _size;
public:
    friend void from_json(const json& j, Rect& p)
    {
        p._topLeft.setX(j.at("x").get<int>());
        p._topLeft.setY(j.at("y").get<int>());
        p._size.setWidth(j.at("width").get<int>());
        p._size.setHeight(j.at("height").get<int>());
    }

    friend void to_json(json& j, const Rect& p)
    {
        j = json{
            {"x", p._topLeft.x()},
            {"y", p._topLeft.y()},
            {"width", p._size.width()},
            {"height", p._size.height()}
        };
    }

};

}
}
#endif // GRAPHICS_RECT_H
