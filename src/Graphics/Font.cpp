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

#include "Font.h"

#include "../ResourceManager.h"
#include "../common/misc.h"
#include "../Utils/Logger.h"
#include "Renderer.h"

#include <SDL_image.h>
#include <json.hpp>
#include <utf8.h>
#include <fstream>
#include <iostream>

using namespace VXHelper::Graphics;

namespace VXHelper
{
namespace Graphics
{

Font::Font()
    : _height(0)
    , _base(0)
{
}

bool Font::load()
{
  cleanup();
  std::string font = std::string("font_" + std::to_string(Renderer::getInstance()->scale) + ".fnt");
  LOG_DEBUG("Loading font file {}", font.c_str());

  // special empty glyph
  _glyphs[0] = Font::Glyph{0, 0, 0, 0, 0, 0, 0, 0, 0};

  std::string path = ResourceManager::getInstance()->getLocalizedPath(font);
  if (ResourceManager::getInstance()->fileExists(path))
  {
    _upscale = 1;
  }
  else
  {
    _upscale = Renderer::getInstance()->scale;
    font = std::string("font_1.fnt");
    path = ResourceManager::getInstance()->getLocalizedPath(font);
  }

  LOG_DEBUG("Loading font file {}", path.c_str());

  std::ifstream fl;
  fl.open(widen(path), std::ifstream::in | std::ifstream::binary);
  if (fl.is_open())
  {
    nlohmann::json fontdef = nlohmann::json::parse(fl);

    _height = fontdef["common"]["lineHeight"].get<uint32_t>();
    _base   = fontdef["common"]["base"].get<uint32_t>();

    for (auto glyph : fontdef["chars"])
    {
      _glyphs[glyph["id"].get<uint32_t>()] = Font::Glyph{
          glyph["id"].get<uint32_t>(),       glyph["page"].get<uint32_t>(),    glyph["x"].get<uint32_t>(),
          glyph["y"].get<uint32_t>(),        glyph["width"].get<uint32_t>(),   glyph["height"].get<uint32_t>(),
          glyph["xadvance"].get<uint32_t>(), glyph["xoffset"].get<uint32_t>(), glyph["yoffset"].get<uint32_t>()};
    }

    for (auto atlas : fontdef["pages"])
    {
      std::string atlaspath = ResourceManager::getInstance()->getLocalizedPath(atlas.get<std::string>());
      SDL_Surface *surf     = IMG_Load(atlaspath.c_str());
      _atlases.push_back(SDL_CreateTextureFromSurface(Renderer::getInstance()->renderer(), surf));
      SDL_FreeSurface(surf);
    }
  }
  else
  {
    LOG_ERROR("Error opening font file {}", path.c_str());
    return false;
  }

  return true;
}

void Font::cleanup()
{
  _height = 0;
  _base   = 0;
  _glyphs.clear();
  for (auto atlas : _atlases)
  {
    SDL_DestroyTexture(atlas);
  }
  _atlases.clear();
  _upscale = 1;
}

Font::~Font()
{
  cleanup();
}

const Font::Glyph &Font::glyph(uint32_t codepoint)
{
  if (_glyphs.find(codepoint) != _glyphs.end())
  {
    return _glyphs.at(codepoint);
  }
  else
  {
    LOG_WARN("No glyph for codepoint {}", codepoint);
    return _glyphs.at(0);
  }
}

SDL_Texture *Font::atlas(uint32_t idx)
{
  return _atlases.at(idx);
}

uint32_t Font::draw(int x, int y, const std::string &text, uint32_t color, bool isShaded)
{
  x *= Renderer::getInstance()->scale;
  y *= Renderer::getInstance()->scale;

  int orgx = x;
  int i    = 0;
  SDL_Rect dstrect;
  SDL_Rect shdrect;
  SDL_Rect srcrect;

  int r, g, b;

  r = ((color >> 16) & 0xFF);
  g = ((color >> 8) & 0xFF);
  b = ((color)&0xFF);

  std::string::const_iterator it = text.begin();
  while (it != text.end() )
  {
    char32_t ch;
    try {
        ch = utf8::next(it, text.end());
    } catch (utf8::invalid_utf8& e)
    {
        LOG_WARNING("UTF8 decoding error: {}", e.what());
        ch = 0x3F;
    }

    Glyph glyph = this->glyph(ch);
    SDL_Texture *atlas  = this->atlas(glyph.atlasid);

    if (_rendering && ch != ' ')
    {
      dstrect.x = x + (glyph.xoffset * _upscale);
      dstrect.y = y + (glyph.yoffset * _upscale);
      dstrect.w = glyph.w * _upscale;
      dstrect.h = glyph.h * _upscale;

      srcrect.x = glyph.x;
      srcrect.y = glyph.y;
      srcrect.w = glyph.w;
      srcrect.h = glyph.h;

      if (Renderer::getInstance()->isClipSet())
      {
        if (_upscale > 1)
          Renderer::getInstance()->clip(srcrect, dstrect);
        else
          Renderer::getInstance()->clipScaled(srcrect, dstrect);
      }
      if (isShaded)
      {
        shdrect.x = x + (glyph.xoffset * _upscale);
        shdrect.y = y + (glyph.yoffset * _upscale + _shadowOffset * Renderer::getInstance()->scale);
        shdrect.w = glyph.w * _upscale;
        shdrect.h = glyph.h * _upscale;
        SDL_SetTextureColorMod(atlas, 0, 0, 0);
        SDL_RenderCopy(Renderer::getInstance()->renderer(), atlas, &srcrect, &shdrect);
        SDL_SetTextureColorMod(atlas, 255, 255, 255);
      }
      SDL_SetTextureColorMod(atlas, r, g, b);
      SDL_RenderCopy(Renderer::getInstance()->renderer(), atlas, &srcrect, &dstrect);
      SDL_SetTextureColorMod(atlas, 255, 255, 255);
    }

    x += glyph.xadvance * _upscale;
    i++;
  }

  // return the final width of the text drawn
  return abs((x - orgx) / Renderer::getInstance()->scale);
}

uint32_t Font::draw(int x, int y, const std::string &text, Color color, bool isShaded)
{
    return draw(x,y, text, color.hex(), isShaded);
}


uint32_t Font::getWidth(const std::string &text)
{
  _rendering = false;

  uint32_t wd = draw(0, 0, text);

  _rendering = true;

  return wd;
}

uint32_t Font::getHeight() const
{
  return _height / ((_upscale == 1) ? Renderer::getInstance()->scale : 1);
}

uint32_t Font::getBase() const
{
  return _base / ((_upscale == 1) ? Renderer::getInstance()->scale : 1);
}

}; // namespace Graphics
}; // namespace VXHelper
