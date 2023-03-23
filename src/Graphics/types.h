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

#ifndef _G_TYPES_H
#define _G_TYPES_H

#include <cstdint>
#include <SDL.h>

namespace VXHelper
{
namespace Graphics
{

struct Color
{
  uint8_t r, g, b;

  Color()
      : r(0)
      , g(0)
      , b(0)
  {
  }

  Color(uint8_t rr, uint8_t gg, uint8_t bb)
      : r(rr)
      , g(gg)
      , b(bb)
  {
  }

  Color(uint32_t hexcolor)
  {
    r = hexcolor >> 16;
    g = hexcolor >> 8;
    b = hexcolor;
  }

  inline bool operator==(const Color& rhs) const
  {
    return (this->r == rhs.r && this->g == rhs.g && this->b == rhs.b);
  }

  inline bool operator!=(const Color& rhs) const
  {
    return (this->r != rhs.r || this->g != rhs.g || this->b != rhs.b);
  }

  inline uint32_t hex()
  {
    return (this->r << 16) + (this->g << 8) + this->b;
  }
};

}
}
#endif
