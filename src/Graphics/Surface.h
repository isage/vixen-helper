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

#ifndef _SURFACE_H
#define _SURFACE_H

#include "../common/basics.h"

#include <SDL.h>
#include <string>
#include "types.h"

namespace VXHelper
{
namespace Graphics
{

class Surface
{
public:
  Surface();
  ~Surface();

  bool loadImage(const std::string &pbm_name, bool use_colorkey = false);
  static Surface *fromFile(const std::string &pbm_name, bool use_colorkey = false);

  int width();
  int height();
  SDL_Texture* texture();

private:
  void cleanup();

  SDL_Texture *_texture;
  int _width;
  int _height;

public:
  int alpha = 255;
};

}; // namespace Graphics
}; // namespace VXHelper

#endif
