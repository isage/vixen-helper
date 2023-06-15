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

#include "../State/DeviceReading.h"

#include <ctime>
#include <string>
#include <vector>
#include <memory>

#include "../Game/Game.h"
#include "../Graphics/Renderer.h"
#include "../ResourceManager.h"
#include "../Settings.h"
#include "../Utils/Logger.h"
#include "../State/Start.h"
#include "../State/FadeOut.h"
#include "../State/Finished.h"

#if defined(__VITA__)
#include <taihen.h>
#include <vixen-helper.h>
#endif

using namespace VXHelper::Graphics;

namespace VXHelper
{
    namespace State
    {
        DeviceReading::DeviceReading(uint16_t vid, uint16_t pid) : State()
        {
            _vid = vid;
            _pid = pid;
        }

        DeviceReading::~DeviceReading()
        {
        }

        void DeviceReading::init()
        {
            if (_initialized) return;
            State::init();

            setModal(true);
            setFullscreen(true);
#if defined(__VITA__)
            vixenhelperStart(_vid, _pid);
            vixenhelperRead(0);
#endif
        }

        void DeviceReading::think(uint32_t delta)
        {
            State::think(delta);
        }

        void DeviceReading::render()
        {
            Renderer::getInstance()->clearScreen(VGA_COLORS[0x08]);
            Renderer::getInstance()->fillRect(0, 0, Renderer::getInstance()->screenWidth, 16, VGA_COLORS[0x1F]);

            Renderer::getInstance()->fillRect(0, Renderer::getInstance()->screenHeight-16, Renderer::getInstance()->screenWidth, Renderer::getInstance()->screenHeight, VGA_COLORS[0x1F]);

            int pzx = (Renderer::getInstance()->screenWidth / 2) - (300/2);

            Renderer::getInstance()->fillRect(pzx, 116, pzx+314, 116+80, VGA_COLORS[1]);
            Renderer::getInstance()->font.draw(pzx, 116+16*0, "╔═════════════Reading═════════════╗", VGA_COLORS[0x0F]);
            Renderer::getInstance()->font.draw(pzx, 116+16*1, "║         Press and hold          ║", VGA_COLORS[0x0F]);
            switch(_state)
            {
                default:
                    Renderer::getInstance()->font.draw(pzx, 116+16*2, "║            Finished!            ║", VGA_COLORS[0x0F]);
                    break;
                case 0:
                    Renderer::getInstance()->font.draw(pzx, 116+16*2, "║            D-PAD Left           ║", VGA_COLORS[0x0F]);
                    break;
                case 1:
                    Renderer::getInstance()->font.draw(pzx, 116+16*2, "║             D-PAD Up            ║", VGA_COLORS[0x0F]);
                    break;
                case 2:
                    Renderer::getInstance()->font.draw(pzx, 116+16*2, "║            D-PAD Down           ║", VGA_COLORS[0x0F]);
                    break;
                case 3:
                    Renderer::getInstance()->font.draw(pzx, 116+16*2, "║            D-PAD Right          ║", VGA_COLORS[0x0F]);
                    break;
                case 4:
                    Renderer::getInstance()->font.draw(pzx, 116+16*2, "║                X                ║", VGA_COLORS[0x0F]);
                    break;
                case 5:
                    Renderer::getInstance()->font.draw(pzx, 116+16*2, "║                O                ║", VGA_COLORS[0x0F]);
                    break;
                case 6:
                    Renderer::getInstance()->font.draw(pzx, 116+16*2, "║             Triangle            ║", VGA_COLORS[0x0F]);
                    break;
                case 7:
                    Renderer::getInstance()->font.draw(pzx, 116+16*2, "║              Square             ║", VGA_COLORS[0x0F]);
                    break;
                case 8:
                    Renderer::getInstance()->font.draw(pzx, 116+16*2, "║         Left analog Up          ║", VGA_COLORS[0x0F]);
                    break;
                case 9:
                    Renderer::getInstance()->font.draw(pzx, 116+16*2, "║         Left analog Down        ║", VGA_COLORS[0x0F]);
                    break;
                case 10:
                    Renderer::getInstance()->font.draw(pzx, 116+16*2, "║         Left analog Left        ║", VGA_COLORS[0x0F]);
                    break;
                case 11:
                    Renderer::getInstance()->font.draw(pzx, 116+16*2, "║        Left analog Right        ║", VGA_COLORS[0x0F]);
                    break;
                case 12:
                    Renderer::getInstance()->font.draw(pzx, 116+16*2, "║         Right analog Up         ║", VGA_COLORS[0x0F]);
                    break;
                case 13:
                    Renderer::getInstance()->font.draw(pzx, 116+16*2, "║         Right analog Down       ║", VGA_COLORS[0x0F]);
                    break;
                case 14:
                    Renderer::getInstance()->font.draw(pzx, 116+16*2, "║         Right analog Left       ║", VGA_COLORS[0x0F]);
                    break;
                case 15:
                    Renderer::getInstance()->font.draw(pzx, 116+16*2, "║        Right analog Right       ║", VGA_COLORS[0x0F]);
                    break;
                case 16:
                    Renderer::getInstance()->font.draw(pzx, 116+16*2, "║               L1                ║", VGA_COLORS[0x0F]);
                    break;
                case 17:
                    Renderer::getInstance()->font.draw(pzx, 116+16*2, "║               R1                ║", VGA_COLORS[0x0F]);
                    break;
                case 18:
                    Renderer::getInstance()->font.draw(pzx, 116+16*2, "║               L2                ║", VGA_COLORS[0x0F]);
                    break;
                case 19:
                    Renderer::getInstance()->font.draw(pzx, 116+16*2, "║               R2                ║", VGA_COLORS[0x0F]);
                    break;
                case 20:
                    Renderer::getInstance()->font.draw(pzx, 116+16*2, "║               L3                ║", VGA_COLORS[0x0F]);
                    break;
                case 21:
                    Renderer::getInstance()->font.draw(pzx, 116+16*2, "║               R3                ║", VGA_COLORS[0x0F]);
                    break;
                case 22:
                    Renderer::getInstance()->font.draw(pzx, 116+16*2, "║             Select              ║", VGA_COLORS[0x0F]);
                    break;
                case 23:
                    Renderer::getInstance()->font.draw(pzx, 116+16*2, "║              Start              ║", VGA_COLORS[0x0F]);
                    break;
                case 24:
                    Renderer::getInstance()->font.draw(pzx, 116+16*2, "║               PS                ║", VGA_COLORS[0x0F]);
                    break;
            }
            Renderer::getInstance()->font.draw(pzx, 116+16*3, "║ and press X, or press O to skip ║", VGA_COLORS[0x0F]);
            Renderer::getInstance()->font.draw(pzx, 116+16*4, "╚═════════════════════════════════╝", VGA_COLORS[0x0F]);

            Renderer::getInstance()->font.draw(4, 0, "V", VGA_COLORS[0x04]);
            Renderer::getInstance()->font.draw(12, 0, "iXEn Helper v0.1", VGA_COLORS[0x15]);

            Renderer::getInstance()->font.draw(4, Renderer::getInstance()->screenHeight-16, "P", VGA_COLORS[0x04]);
            Renderer::getInstance()->font.draw(12, Renderer::getInstance()->screenHeight-16, "ress and hold button on usb gamepad then press X or press O to skip", VGA_COLORS[0x15]);
        }

        void DeviceReading::onKeyDown(Event::Keyboard* event)
        {
            auto game = Game::Game::getInstance();

            switch (event->keyCode())
            {
                case SDLK_z:
                    game->pushState(new FadeOut(new Finished()));
                    break;
                case SDLK_x:
                    _state++;
                    if (_state > 24)
                        game->pushState(new FadeOut(new Finished()));
                    break;
                default:
                  break;
            }
        }

        void DeviceReading::onButtonDown(Event::Gamepad* event)
        {
            auto game = Game::Game::getInstance();

            switch (event->button())
            {
                case SDL_CONTROLLER_BUTTON_B:
                    _state++;
                    if (_state > 24)
                    {
#if defined(__VITA__)
                        vixenhelperStop();
#endif
                        game->pushState(new FadeOut(new Finished()));
                    }
                    break;
                case SDL_CONTROLLER_BUTTON_A:
                    _state++;
                    if (_state > 24)
                    {
#if defined(__VITA__)
                        vixenhelperStop();
#endif
                        game->pushState(new FadeOut(new Finished()));
                    }
                    else
                    {
#if defined(__VITA__)
                      vixenhelperRead(_state);
#endif
                    }
                    break;
                default:
                  break;
            }
        }

    }
}
