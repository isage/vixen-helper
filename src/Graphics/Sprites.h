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

#ifndef _SPRITES_H
#define _SPRITES_H

#include "Surface.h"
#include "Sprite.h"
#include "types.h"


#include <vector>
#include <string>
#include <map>
#include <cstdint>

namespace VXHelper
{
namespace Graphics
{

class Sprites
{
public:
  Sprites();
  ~Sprites();

  bool init();
  void close();
  void flushSheets();
  Sprite& getSprite(const std::string& name);

  void blitSprite(const std::string& name, int x, int y, size_t frame, Sprite::Dir dir, int xoff, int yoff, int wd, int ht, int alpha = 255);
  void blitSpriteMirrored(const std::string& name, int x, int y, size_t frame, Sprite::Dir dir, int xoff, int yoff, int wd, int ht, int alpha = 255);

  template<class T,class T2>
  void drawSprite(const std::string& name, T x, T2 y, size_t frame = 0, Sprite::Dir dir = Sprite::Dir::LEFT)
  {
    blitSprite(name, static_cast<int>(x), static_cast<int>(y), frame, dir, 0, 0, _sprites[name].width(), _sprites[name].height());
  }

  template<class T,class T2>
  void drawSpriteMirrored(const std::string& name, T x, T2 y, size_t frame = 0, Sprite::Dir dir = Sprite::Dir::LEFT)
  {
    blitSpriteMirrored(name, static_cast<int>(x), static_cast<int>(y), frame, dir, 0, 0, _sprites[name].width(), _sprites[name].height());
  }

private:
  std::map<std::string, Sprite> _sprites;
  std::map<std::string, Surface*> _sheets;

  void _loadSheetIfNeeded(std::string& sheet);
};
}; // namespace Graphics
}; // namespace VXHelper

#endif
