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

#include "Sprites.h"

#include "../ResourceManager.h"
#include "Renderer.h"
#include "Surface.h"
#include "Sprite.h"
#include "../Utils/Logger.h"
#include "../common/misc.h"

#include <cstring>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <json.hpp>

namespace VXHelper
{
namespace Graphics
{

Sprites::Sprites() {}

Sprites::~Sprites()
{
  close();
}

bool Sprites::init()
{
  for(auto& p: std::filesystem::directory_iterator(ResourceManager::getInstance()->getPathForDir("sprites")))
  {
    LOG_DEBUG("Loading sprite: {}", p.path().string());
    std::ifstream fl;

    fl.open(widen(p.path()), std::ifstream::in | std::ifstream::binary);
    if (fl.is_open())
    {
        nlohmann::json json = nlohmann::json::parse(fl);
        Sprite sp = json;
        _sprites.emplace(sp.name(), std::move(sp));
        LOG_DEBUG("Sprite: {}, tileset: {}, frames: {}", sp.name(), sp.spritesheet(), sp.frames());
        fl.close();
    }
  }

  return true;
}

void Sprites::close()
{
  flushSheets();
  _sheets.clear();
}

void Sprites::flushSheets()
{
  for (auto& [key, value]: _sheets)
  {
    delete value;
  }
}

void Sprites::_loadSheetIfNeeded(std::string& sheet)
{
  if (_sheets.find(sheet) == _sheets.end())
  {
    _sheets.emplace(sheet, Surface::fromFile(ResourceManager::getInstance()->getLocalizedPath("bmp/"+sheet), true));
  }
}

Sprite& Sprites::getSprite(const std::string& name)
{
    return _sprites.at(name);
}

// master sprite drawing function
void Sprites::blitSprite(const std::string& name, int x, int y, size_t frame, Sprite::Dir dir, int xoff, int yoff, int wd, int ht, int alpha)
{
  if (_sprites.find(name) != _sprites.end())
  {
    _loadSheetIfNeeded(_sprites.at(name).spritesheet());

    if (frame < _sprites.at(name).frames())
    {
      SpriteDir sprdir = _sprites.at(name).dir(frame, dir);

      _sheets.at(_sprites.at(name).spritesheet())->alpha = alpha;

      Renderer::getInstance()->drawSurface(_sheets.at(_sprites.at(name).spritesheet()), x, y, (sprdir.sheetOffset.x() + xoff),
                (sprdir.sheetOffset.y() + yoff), wd, ht);

      _sheets.at(_sprites.at(name).spritesheet())->alpha = 255;
    }
  }
}

void Sprites::blitSpriteMirrored(const std::string& name, int x, int y, size_t frame, Sprite::Dir dir, int xoff, int yoff, int wd, int ht, int alpha)
{
  if (_sprites.find(name) != _sprites.end())
  {
    _loadSheetIfNeeded(_sprites.at(name).spritesheet());

    if (frame < _sprites.at(name).frames())
    {
      SpriteDir sprdir = _sprites.at(name).dir(frame, dir);

      _sheets.at(_sprites.at(name).spritesheet())->alpha = alpha;

      Renderer::getInstance()->drawSurfaceMirrored(_sheets.at(_sprites.at(name).spritesheet()), x, y, (sprdir.sheetOffset.x() + xoff),
                (sprdir.sheetOffset.y() + yoff), wd, ht);

      _sheets.at(_sprites.at(name).spritesheet())->alpha = 255;
    }
  }
}

}; // namespace Graphics
}; // namespace VXHelper