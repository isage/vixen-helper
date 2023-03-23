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

#include <algorithm>
#include <cassert>
#include "../ResourceManager.h"
#include "../version.h"
#include "Renderer.h"
#include "../Utils/Logger.h"

#include <SDL.h>
#include <SDL_image.h>
#include <cstdlib>
#include <sstream>
#include <iomanip>

namespace VXHelper
{
namespace Graphics
{

Renderer::Renderer() {}
Renderer::~Renderer() {}

Renderer *Renderer::getInstance()
{
  return Base::Singleton<Renderer>::get();
}

bool Renderer::init(int resolution)
{
  if (!initVideo())
    return false;

  if (!font.load())
    return false;

  if (!sprites.init())
    return false;

  return true;
}

void Renderer::close()
{
  LOG_INFO("Renderer::Close()");
  font.cleanup();
  sprites.close();
  SDL_ShowCursor(true);
  SDL_DestroyWindow(_window);
  _window = NULL;
}

bool Renderer::isWindowVisible()
{
  Uint32 flags = SDL_GetWindowFlags(_window);

  return (flags & SDL_WINDOW_SHOWN) && !(flags & SDL_WINDOW_MINIMIZED) // SDL_APPACTIVE
         && (flags & SDL_WINDOW_INPUT_FOCUS);                          // SDL_APPINPUTFOCUS
}

bool Renderer::initVideo()
{
  uint32_t window_flags = SDL_WINDOW_SHOWN;

  uint32_t width  = 960;
  uint32_t height = 544;

  if (_window)
  {
    LOG_WARN("second call to Renderer::InitVideo()");
    return false;
  }

  LOG_DEBUG("SDL_CreateWindow: {}x{}", width, height);
  _window = SDL_CreateWindow(VERSION, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, window_flags);

  if (!_window)
  {
    LOG_ERROR("Renderer::initVideo: error setting video mode (SDL_CreateWindow: {})", SDL_GetError());
    return false;
  }

  if (!_renderer)
  {
    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
  }
  if (!_renderer)
  {
    LOG_ERROR("Renderer::initVideo: error setting video mode (SDL_CreateRenderer: {})", SDL_GetError());
    return false;
  }

  SDL_RendererInfo info;
  if (SDL_GetRendererInfo(_renderer, &info))
  {
    LOG_ERROR("Renderer::initVideo: SDL_GetRendererInfo failed: {}", SDL_GetError());
    return false;
  }

  LOG_INFO("Renderer::initVideo: using: {} renderer", info.name);

  return true;
}

bool Renderer::flushAll()
{
  LOG_DEBUG("Renderer::flushAll()");
  SDL_RenderPresent(_renderer);
  sprites.flushSheets();
  if (!font.load())
    return false;

  return true;
}


void Renderer::showLoadingScreen()
{
}

SDL_Renderer* Renderer::renderer()
{
  return _renderer;
}

SDL_Window* Renderer::window()
{
  return _window;
}

void Renderer::flip()
{
//  LOG_INFO("===FLIPPING===\n");
  SDL_RenderPresent(_renderer);
//  LOG_INFO("===FLIPPED===\n");
}

// blit the specified portion of the surface to the screen
void Renderer::drawSurface(Surface *src, int dstx, int dsty, int srcx, int srcy, int wd, int ht)
{
  assert(_renderer);
  assert(src->texture());

  SDL_Rect srcrect, dstrect;

  srcrect.x = srcx * scale;
  srcrect.y = srcy * scale;
  srcrect.w = wd * scale;
  srcrect.h = ht * scale;

  dstrect.x = dstx * scale;
  dstrect.y = dsty * scale;
  dstrect.w = srcrect.w;
  dstrect.h = srcrect.h;

  if (_need_clip)
    clipScaled(srcrect, dstrect);

  SDL_SetTextureAlphaMod(src->texture(), src->alpha);
  if (SDL_RenderCopy(_renderer, src->texture(), &srcrect, &dstrect))
  {
    LOG_ERROR("Renderer::drawSurface: SDL_RenderCopy failed: {}", SDL_GetError());
  }
}

// blit the specified portion of the surface to the screen
void Renderer::drawSurfaceMirrored(Surface *src, int dstx, int dsty, int srcx, int srcy, int wd, int ht)
{
  assert(_renderer);
  assert(src->texture());

  SDL_Rect srcrect, dstrect;

  srcrect.x = srcx * scale;
  srcrect.y = srcy * scale;
  srcrect.w = wd * scale;
  srcrect.h = ht * scale;

  dstrect.x = dstx * scale;
  dstrect.y = dsty * scale;
  dstrect.w = srcrect.w;
  dstrect.h = srcrect.h;

  if (_need_clip)
    clipScaled(srcrect, dstrect);

  SDL_SetTextureAlphaMod(src->texture(), src->alpha);
  if (SDL_RenderCopyEx(_renderer, src->texture(), &srcrect, &dstrect, 0, NULL, SDL_FLIP_HORIZONTAL))
  {
    LOG_ERROR("Renderer::drawSurface: SDL_RenderCopy failed: {}", SDL_GetError());
  }
}

// blit the specified surface across the screen in a repeating pattern
void Renderer::blitPatternAcross(Surface *sfc, int x_dst, int y_dst, int y_src, int height)
{
  SDL_Rect srcrect, dstrect;

  srcrect.x = 0;
  srcrect.w = sfc->width() * scale;
  srcrect.y = (y_src * scale);
  srcrect.h = (height * scale);

  dstrect.w = srcrect.w;
  dstrect.h = srcrect.h;

  int x      = (x_dst * scale);
  int y      = (y_dst * scale);
  int destwd = screenWidth * scale;

  assert(!_need_clip && "clip for blitpattern is not implemented");

  do
  {
    dstrect.x = x;
    dstrect.y = y;
    SDL_RenderCopy(_renderer, sfc->texture(), &srcrect, &dstrect);
    x += sfc->width()  * scale;
  } while (x < destwd);
}

void Renderer::drawLine(int x1, int y1, int x2, int y2, Color color)
{
  SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
  SDL_RenderDrawLine(_renderer, x1 * scale, y1 * scale, x2 * scale, y2 * scale);
}

void Renderer::drawRect(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
  SDL_Rect rects[4] = {{x1 * scale, y1 * scale, ((x2 - x1) + 1) * scale, scale},
  {x1 * scale, y2 * scale, ((x2 - x1) + 1) * scale, scale},
  {x1 * scale, y1 * scale, scale, ((y2 - y1) + 1) * scale},
  {x2 * scale, y1 * scale, scale, ((y2 - y1) + 1) * scale}};

  SDL_SetRenderDrawColor(_renderer, r, g, b, a);
  SDL_RenderFillRects(_renderer, rects, 4);
}

void Renderer::fillRect(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
  SDL_Rect rect;

  rect.x = x1 * scale;
  rect.y = y1 * scale;
  rect.w = ((x2 - x1) + 1) * scale;
  rect.h = ((y2 - y1) + 1) * scale;

  SDL_SetRenderDrawBlendMode(_renderer,SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(_renderer, r, g, b, a);
  SDL_RenderFillRect(_renderer, &rect);
}

void Renderer::tintScreen()
{
  SDL_SetRenderDrawBlendMode(_renderer,SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 150);
  SDL_RenderFillRect(_renderer, NULL);
}

void Renderer::drawPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b)
{
  fillRect(x, y, x, y, r, g, b);
}

void Renderer::clearScreen(uint8_t r, uint8_t g, uint8_t b)
{
  SDL_SetRenderDrawColor(_renderer, r, g, b, SDL_ALPHA_OPAQUE);
  SDL_RenderFillRect(_renderer, NULL);
}

void Renderer::setClip(int x, int y, int w, int h)
{
  _need_clip = true;

  _clip_rect.x = x * scale;
  _clip_rect.y = y * scale;
  _clip_rect.w = w * scale;
  _clip_rect.h = h * scale;
}

void Renderer::clearClip()
{
  _need_clip = false;
}

bool Renderer::isClipSet()
{
  return _need_clip;
}

void Renderer::clip(SDL_Rect &srcrect, SDL_Rect &dstrect)
{
  int w,h;
  int dx, dy;

  w = dstrect.w;
  h = dstrect.h;

  dx = _clip_rect.x - dstrect.x;
  if (dx > 0)
  {
    w -= dx;
    dstrect.x += dx;
    srcrect.x += dx / scale;
  }
  dx = dstrect.x + w - _clip_rect.x - _clip_rect.w;
  if (dx > 0)
    w -= dx;

  dy = _clip_rect.y - dstrect.y;
  if (dy > 0)
  {
    h -= dy;
    dstrect.y += dy;
    srcrect.y += dy / scale;
  }
  dy = dstrect.y + h - _clip_rect.y - _clip_rect.h;
  if (dy > 0)
    h -= dy;

  dstrect.w = w;
  dstrect.h = h;
  srcrect.w = w / scale;
  srcrect.h = h / scale;
}

void Renderer::clipScaled(SDL_Rect &srcrect, SDL_Rect &dstrect)
{
  int w, h;
  int dx, dy;

  w = dstrect.w;
  h = dstrect.h;

  dx = _clip_rect.x - dstrect.x;
  if (dx > 0)
  {
    w -= dx;
    dstrect.x += dx;
    srcrect.x += dx;
  }
  dx = dstrect.x + w - _clip_rect.x - _clip_rect.w;
  if (dx > 0)
    w -= dx;

  dy = _clip_rect.y - dstrect.y;
  if (dy > 0)
  {
    h -= dy;
    dstrect.y += dy;
    srcrect.y += dy;
  }
  dy = dstrect.y + h - _clip_rect.y - _clip_rect.h;
  if (dy > 0)
    h -= dy;

  dstrect.w = srcrect.w = w;
  dstrect.h = srcrect.h = h;
}

void Renderer::saveScreenshot()
{
  return;
}


};
};
