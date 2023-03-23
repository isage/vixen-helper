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

#ifndef _RENDERER_H
#define _RENDERER_H

#include "../Base/Singleton.h"
#include "types.h"
#include "Surface.h"
#include "Font.h"
#include "Sprites.h"

#include <SDL.h>


namespace VXHelper
{
namespace Graphics
{

const Color COLOR_DK_BLUE(0,0,0x21);
const Color COLOR_BLACK(0,0,0);
const Color COLOR_WHITE(0xFF,0xFF,0xFF);
#include "vga_palette.h"

class Renderer
{
  public:
    static Renderer *getInstance();

    int screenWidth = 960;
    int screenHeight = 544;
    bool widescreen = true;
    int scale = 1;

    bool init(int resolution);
    void close();

    bool isWindowVisible();

    bool initVideo();

    bool flushAll();

    void showLoadingScreen();

    void drawSurface(Surface *src, int x, int y);
    void drawSurface(Surface *src, int dstx, int dsty, int srcx, int srcy, int wd, int ht);
    void drawSurfaceMirrored(Surface *src, int dstx, int dsty, int srcx, int srcy, int wd, int ht);

    void blitPatternAcross(Surface *sfc, int x_dst, int y_dst, int y_src, int height);

    void clearScreen(Color color);
    void clearScreen(uint8_t r, uint8_t g, uint8_t b);

    void drawLine(int x1, int y1, int x2, int y2, Color color);

    void drawRect(int x1, int y1, int x2, int y2, Color color);

    void drawRect(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a = SDL_ALPHA_OPAQUE);
    void drawRect(SDL_Rect *rect, uint8_t r, uint8_t g, uint8_t b);
    void drawRect(SDL_Rect *rect, Color color);

    void fillRect(int x1, int y1, int x2, int y2, Color color);
    void fillRect(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a = SDL_ALPHA_OPAQUE);
    void fillRect(SDL_Rect *rect, uint8_t r, uint8_t g, uint8_t b);
    void fillRect(SDL_Rect *rect, Color color);

    void drawPixel(int x, int y, Color color);
    void drawPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);

    void setClip(int x, int y, int w, int h);
    void setClip(SDL_Rect *rect);
    void clearClip();
    bool isClipSet();
    void clip(SDL_Rect &srcrect, SDL_Rect &dstrect);
    void clipScaled(SDL_Rect &srcrect, SDL_Rect &dstrect);

    void saveScreenshot();

    void tintScreen();
    void flip();
    SDL_Renderer* renderer();
    SDL_Window* window();
    Font font;
    Sprites sprites;

  private:
    SDL_Window *_window = nullptr;
    SDL_Renderer *_renderer = nullptr;
    bool _need_clip = false;
    SDL_Rect _clip_rect;

  protected:
    friend class Base::Singleton<Renderer>;

    Renderer();
    ~Renderer();
    Renderer(const Renderer &) = delete;
    Renderer &operator=(const Renderer &) = delete;
};

void inline Renderer::drawSurface(Surface *src, int dstx, int dsty)
{
  drawSurface(src, dstx, dsty, 0, 0, src->width(), src->height());
}

void inline Renderer::drawRect(int x1, int y1, int x2, int y2, Color color)
{
  drawRect(x1, y1, x2, y2, color.r, color.g, color.b, 255);
}

void inline Renderer::drawRect(SDL_Rect *rect, uint8_t r, uint8_t g, uint8_t b)
{
  drawRect(rect->x, rect->y, rect->x + (rect->w - 1), rect->y + (rect->h - 1), r, g, b, 255);
}

void inline Renderer::drawRect(SDL_Rect *rect, Color color)
{
  drawRect(rect->x, rect->y, rect->x + (rect->w - 1), rect->y + (rect->h - 1), color.r, color.g, color.b,  255);
}

void inline Renderer::fillRect(int x1, int y1, int x2, int y2, Color color)
{
  fillRect(x1, y1, x2, y2, color.r, color.g, color.b,  255);
}

void inline Renderer::fillRect(SDL_Rect *rect, uint8_t r, uint8_t g, uint8_t b)
{
  fillRect(rect->x, rect->y, rect->x + (rect->w - 1), rect->y + (rect->h - 1), r, g, b,  255);
}

void inline Renderer::fillRect(SDL_Rect *rect, Color color)
{
  fillRect(rect->x, rect->y, rect->x + (rect->w - 1), rect->y + (rect->h - 1), color.r, color.g, color.b,  255);
}

void inline Renderer::drawPixel(int x, int y, Color color)
{
  drawPixel(x, y, color.r, color.g, color.b);
}

void inline Renderer::clearScreen(Color color)
{
  clearScreen(color.r, color.g, color.b);
}

void inline Renderer::setClip(SDL_Rect *rect)
{
  setClip(rect->x, rect->y, rect->w, rect->h);
}



}; // namespace Graphics
}; // namespace VXHelper
#endif
