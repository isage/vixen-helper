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

#ifndef GRAPHICS_SPRITE_H
#define GRAPHICS_SPRITE_H


#include "../Graphics/Point.h"
#include "../Graphics/Rect.h"
#include "../Graphics/Size.h"

#include <json.hpp>
#include <vector>
#include <cstdint>

#define SIF_MAX_DIRS 4

using nlohmann::json;

namespace VXHelper
{

namespace Graphics
{

struct SpriteDir
{
  Point sheetOffset;
  Point drawPoint;
};

struct SpriteFrame
{
  SpriteDir dirs[SIF_MAX_DIRS];
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SpriteDir, sheetOffset, drawPoint)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SpriteFrame, dirs)

class Sprite
{
public:
  enum class Dir
  {
    LEFT = 0,
    RIGHT = 1,
    UP = 2,
    DOWN = 3
  };

  Sprite();
  ~Sprite();

  SpriteDir& dir(int frame, Dir dir);
  int width();
  int height();
  std::string& spritesheet();
  std::string& name();
  size_t frames();

private:
  Size _size;

  std::string _name;
  std::string _spritesheet;

  int _ndirs;

  std::vector<SpriteFrame> _frames;

  Rect _bbox;
  Rect _solidbox;

  Point _spawnPoint;

  std::vector<Point> _block_l;
  std::vector<Point> _block_r;
  std::vector<Point> _block_u;
  std::vector<Point> _block_d;

public:
  friend void from_json(const json& j, Sprite& s) {
    j["sheet"].get_to(s._spritesheet);
    j["name"].get_to(s._name);

    // TODO: get rid of this shit
    j["ndirs"].get_to(s._ndirs);

    j["block_d"].get_to(s._block_d);
    j["block_u"].get_to(s._block_u);
    j["block_l"].get_to(s._block_l);
    j["block_r"].get_to(s._block_r);

    j["frames"].get_to(s._frames);

    s._size.setWidth(j["width"].get<int>());
    s._size.setHeight(j["height"].get<int>());

    j["bbox"].get_to(s._bbox);
    j["solidbox"].get_to(s._solidbox);
    j["spawnpoint"].get_to(s._spawnPoint);
  }


};



}
}

#endif
