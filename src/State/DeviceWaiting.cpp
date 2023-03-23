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

#include "../State/DeviceWaiting.h"

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
#include "../State/DeviceList.h"

using namespace VXHelper::Graphics;

namespace VXHelper
{
    namespace State
    {
        DeviceWaiting::DeviceWaiting() : State()
        {
        }

        DeviceWaiting::~DeviceWaiting()
        {
        }

        void DeviceWaiting::init()
        {
            if (_initialized) return;
            State::init();

            setModal(true);
            setFullscreen(true);
        }

        void DeviceWaiting::think(uint32_t delta)
        {
            State::think(delta);
        }

        void DeviceWaiting::render()
        {
            Renderer::getInstance()->clearScreen(VGA_COLORS[0x08]);
            Renderer::getInstance()->fillRect(0, 0, Renderer::getInstance()->screenWidth, 16, VGA_COLORS[0x1F]);

            Renderer::getInstance()->fillRect(0, Renderer::getInstance()->screenHeight-16, Renderer::getInstance()->screenWidth, Renderer::getInstance()->screenHeight, VGA_COLORS[0x1F]);

            int pzx = (Renderer::getInstance()->screenWidth / 2) - (212/2);

            Renderer::getInstance()->fillRect(pzx, 116, pzx+212, 116+64, VGA_COLORS[1]);
            Renderer::getInstance()->font.draw(pzx, 116+16*0, "╔═══════Waiting════════╗", VGA_COLORS[0x0F]);
            Renderer::getInstance()->font.draw(pzx, 116+16*1, "║                      ║", VGA_COLORS[0x0F]);
            Renderer::getInstance()->font.draw(pzx, 116+16*2, "║                      ║", VGA_COLORS[0x0F]);
            Renderer::getInstance()->font.draw(pzx, 116+16*3, "╚══════════════════════╝", VGA_COLORS[0x0F]);

            Renderer::getInstance()->font.draw(pzx+48, 116+16*1, "Please connect", VGA_COLORS[0x0F]);
            Renderer::getInstance()->font.draw(pzx+80, 116+16*2, "device", VGA_COLORS[0x0F]);

            Renderer::getInstance()->font.draw(4, 0, "V", VGA_COLORS[0x04]);
            Renderer::getInstance()->font.draw(12, 0, "iXEn Helper v0.1", VGA_COLORS[0x15]);

            Renderer::getInstance()->font.draw(4, Renderer::getInstance()->screenHeight-16, "C", VGA_COLORS[0x04]);
            Renderer::getInstance()->font.draw(12, Renderer::getInstance()->screenHeight-16, "onnect device and press X...", VGA_COLORS[0x15]);
        }

        void DeviceWaiting::onKeyDown(Event::Keyboard* event)
        {
            auto game = Game::Game::getInstance();

            switch (event->keyCode())
            {
                case SDLK_z:
                    game->pushState(new FadeOut(new Start()));
                    break;
                case SDLK_x:
                    game->pushState(new FadeOut(new DeviceList()));
                    break;
                default:
                  break;
            }
        }

        void DeviceWaiting::onButtonDown(Event::Gamepad* event)
        {
            auto game = Game::Game::getInstance();

            switch (event->button())
            {
                case SDL_CONTROLLER_BUTTON_B:
                    game->pushState(new FadeOut(new Start()));
                    break;
                case SDL_CONTROLLER_BUTTON_A:
                    game->pushState(new FadeOut(new DeviceList()));
                    break;
                default:
                  break;
            }
        }

    }
}
