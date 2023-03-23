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

#include "Surface.h"
#include "Renderer.h"
#include "../Utils/Logger.h"
#include "zoom.h"

namespace VXHelper
{
namespace Graphics
{

Surface::Surface()
    : _texture(nullptr)
    , _width(0)
    , _height(0)
{
}

Surface::~Surface()
{
  cleanup();
}

// load the surface from a .pbm or bitmap file
bool Surface::loadImage(const std::string &pbm_name, bool use_colorkey)
{
  cleanup();

  SDL_Surface *image = SDL_LoadBMP(pbm_name.c_str());
  if (!image)
  {
    LOG_ERROR("Surface::LoadImage: load failed of '{}'! {}", pbm_name, SDL_GetError());
    return false;
  }

  _width = image->w * Renderer::getInstance()->scale;
  _height = image->h * Renderer::getInstance()->scale;

  SDL_Surface *image_scaled = SDL_ZoomSurface(image, Renderer::getInstance()->scale);
  SDL_FreeSurface(image);

  if (use_colorkey)
  {
    SDL_SetColorKey(image_scaled, SDL_TRUE, SDL_MapRGB(image_scaled->format, 0, 0, 0));
  }

  _texture = SDL_CreateTextureFromSurface(Renderer::getInstance()->renderer(), image_scaled);

  SDL_FreeSurface(image_scaled);

  if (!_texture)
  {
    LOG_ERROR("Surface::LoadImage: SDL_CreateTextureFromSurface failed: {}", SDL_GetError());
    return false;
  }

  return true;
}

Surface *Surface::fromFile(const std::string &pbm_name, bool use_colorkey)
{
  Surface *sfc = new Surface;
  if (!sfc->loadImage(pbm_name, use_colorkey))
  {
    delete sfc;
    return nullptr;
  }

  return sfc;
}

int Surface::width()
{
  return _width / Renderer::getInstance()->scale;
}

int Surface::height()
{
  return _height / Renderer::getInstance()->scale;
}

SDL_Texture* Surface::texture()
{
  return _texture;
}

void Surface::cleanup()
{
  if (_texture)
  {
    SDL_DestroyTexture(_texture);
    _texture = nullptr;
  }
}

}; // namespace Graphics
}; // namespace VXHelper
