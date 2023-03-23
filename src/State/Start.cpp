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

#include "../State/Start.h"

#include <ctime>
#include <string>
#include <vector>
#include <memory>

#include "../Game/Game.h"
#include "../Graphics/Renderer.h"
#include "../ResourceManager.h"
#include "../Settings.h"
#include "../Utils/Logger.h"
#include "../State/FadeOut.h"
#include "../State/Exit.h"
#include "../State/DeviceWaiting.h"

using namespace VXHelper::Graphics;

namespace VXHelper
{
    namespace State
    {
        Start::Start() : State()
        {
        }

        Start::~Start()
        {
        }

        void Start::init()
        {
            if (_initialized) return;
            State::init();

            setModal(true);
            setFullscreen(true);
        }

        void Start::think(uint32_t delta)
        {
            State::think(delta);
        }

        void Start::render()
        {
            Renderer::getInstance()->clearScreen(VGA_COLORS[0x08]);
            Renderer::getInstance()->fillRect(0, 0, Renderer::getInstance()->screenWidth, 16, VGA_COLORS[0x1F]);

            Renderer::getInstance()->fillRect(0, Renderer::getInstance()->screenHeight-16, Renderer::getInstance()->screenWidth, Renderer::getInstance()->screenHeight, VGA_COLORS[0x1F]);

            int pzx = (Renderer::getInstance()->screenWidth / 2) - (180/2);

            Renderer::getInstance()->fillRect(pzx, 116, pzx+180, 116+64, VGA_COLORS[1]);
            Renderer::getInstance()->font.draw(pzx, 116+16*0, "╔══════ MENU ══════╗", VGA_COLORS[0x1F]);
            Renderer::getInstance()->font.draw(pzx, 116+16*1, "║                  ║", VGA_COLORS[0x1F]);
            Renderer::getInstance()->font.draw(pzx, 116+16*2, "║                  ║", VGA_COLORS[0x1F]);
            Renderer::getInstance()->font.draw(pzx, 116+16*3, "╚══════════════════╝", VGA_COLORS[0x1F]);

            Renderer::getInstance()->font.draw(pzx+72, 116+16*1, "START", _menuPos == 0 ? VGA_COLORS[0x04] : VGA_COLORS[0x0F]);
            Renderer::getInstance()->font.draw(pzx+72, 116+16*2, "QUIT", _menuPos == 1 ? VGA_COLORS[0x04] : VGA_COLORS[0x0F]);

            Renderer::getInstance()->font.draw(4, 0, "V", VGA_COLORS[0x04]);
            Renderer::getInstance()->font.draw(12, 0, "iXEn Helper v0.1", VGA_COLORS[0x15]);

//            Renderer::getInstance()->font.draw(4, Renderer::getInstance()->screenHeight-16, "W", VGA_COLORS[0x04]);
//            Renderer::getInstance()->font.draw(12, Renderer::getInstance()->screenHeight-16, "aiting for device...", VGA_COLORS[0x15]);

            switch (_menuPos)
            {
              case 0:
              default:
                Renderer::getInstance()->font.draw(pzx+56, 116+16*1, "\x10", VGA_COLORS[0x0F]);
                break;
              case 1:
                Renderer::getInstance()->font.draw(pzx+56, 116+16*2, "\x10", VGA_COLORS[0x0F]);
                break;
            }
        }

        void Start::onKeyDown(Event::Keyboard* event)
        {
            auto game = Game::Game::getInstance();

            switch (event->keyCode())
            {
                case SDLK_z:
                    LOG_DEBUG("ZEE PRESSED");
                    switch(_menuPos)
                    {
                        case 0:
//                          game->setState(new DeviceWaiting());
                          game->pushState(new FadeOut(new DeviceWaiting()));
                          break;
                        case 1:
//                          game->setState(new Exit());
                          game->pushState(new FadeOut(new Exit()));
                          break;
                    }
                    break;
                case SDLK_DOWN:
                    _menuPos++;
                    if (_menuPos > 1) _menuPos = 0;
                    break;
                case SDLK_UP:
                    _menuPos--;
                    if (_menuPos <0 ) _menuPos = 1;
                    break;
                default:
                  break;
            }
        }

        void Start::onButtonDown(Event::Gamepad* event)
        {
            auto game = Game::Game::getInstance();

            switch (event->button())
            {
                case SDL_CONTROLLER_BUTTON_A:
                    switch(_menuPos)
                    {
                        case 0:
                          game->pushState(new FadeOut(new DeviceWaiting()));
                          break;
                        case 1:
                          game->pushState(new FadeOut(new Exit()));
                          break;
                    }
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                    _menuPos++;
                    if (_menuPos > 1) _menuPos = 0;
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_UP:
                    _menuPos--;
                    if (_menuPos <0 ) _menuPos = 1;
                    break;
                default:
                  break;
            }
        }

    }
}
