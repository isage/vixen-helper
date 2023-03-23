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

#ifndef FONT_H
#define FONT_H

#include <SDL.h>
#include <map>
#include <string>
#include <vector>
#include "types.h"

namespace VXHelper
{
namespace Graphics
{

class Font
{
public:
  struct Glyph
  {
    uint32_t glyph_id;
    uint32_t atlasid;
    uint32_t x;
    uint32_t y;
    uint32_t w;
    uint32_t h;
    uint32_t xadvance;
    uint32_t xoffset;
    uint32_t yoffset;
  };

  Font();
  ~Font();
  bool load();
  void cleanup();
  const Font::Glyph &glyph(uint32_t codepoint);
  SDL_Texture *atlas(uint32_t idx);
  uint32_t draw(int x, int y, const std::string &text, uint32_t color = 0xFFFFFF, bool isShaded = false);
  uint32_t draw(int x, int y, const std::string &text, Color color, bool isShaded = false);
  uint32_t getWidth(const std::string &text);
  uint32_t getHeight() const;
  uint32_t getBase() const;


private:
  std::vector<SDL_Texture *> _atlases;
  std::map<uint32_t, Glyph> _glyphs;
  uint32_t _height;
  uint32_t _base;
  uint32_t _upscale;
  bool _rendering = true;
  const uint8_t _shadowOffset = 1;
};

}; // namespace VXHelper
}; // namespace Graphics

#endif
