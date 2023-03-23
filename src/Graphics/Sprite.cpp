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

#include "Sprite.h"

#include "../Graphics/Point.h"
#include "../Graphics/Rect.h"

namespace VXHelper
{

namespace Graphics
{

  Sprite::Sprite() {}
  Sprite::~Sprite(){}

  SpriteDir& Sprite::dir(int frame, Dir dir)
  {
    int _dir = (int)dir;
    _dir %= _ndirs;
    return _frames.at(frame).dirs[_dir];
  }

  int Sprite::width()
  {
    return _size.width();
  }

  int Sprite::height()
  {
    return _size.height();
  }

  size_t Sprite::frames()
  {
    return _frames.size();
  }

  std::string& Sprite::spritesheet()
  {
    return _spritesheet;
  }

  std::string& Sprite::name()
  {
    return _name;
  }
}
}
