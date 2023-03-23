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

#include "../State/Finished.h"

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


using namespace VXHelper::Graphics;

namespace VXHelper
{
    namespace State
    {
        Finished::Finished() : State()
        {
        }

        Finished::~Finished()
        {
        }

        void Finished::init()
        {
            if (_initialized) return;
            State::init();

            setModal(true);
            setFullscreen(true);
        }

        void Finished::think(uint32_t delta)
        {
            State::think(delta);
        }

        void Finished::render()
        {
            Renderer::getInstance()->clearScreen(VGA_COLORS[0x08]);
            Renderer::getInstance()->fillRect(0, 0, Renderer::getInstance()->screenWidth, 16, VGA_COLORS[0x1F]);

            Renderer::getInstance()->fillRect(0, Renderer::getInstance()->screenHeight-16, Renderer::getInstance()->screenWidth, Renderer::getInstance()->screenHeight, VGA_COLORS[0x1F]);

            int pzx = (Renderer::getInstance()->screenWidth / 2) - (212/2);

            Renderer::getInstance()->fillRect(pzx, 116, pzx+212, 116+64, VGA_COLORS[1]);
            Renderer::getInstance()->font.draw(pzx, 116+16*0, "╔═══════Finished═══════╗", VGA_COLORS[0x0F]);
            Renderer::getInstance()->font.draw(pzx, 116+16*1, "║        Hurray!       ║", VGA_COLORS[0x0F]);
            Renderer::getInstance()->font.draw(pzx, 116+16*2, "║                      ║", VGA_COLORS[0x0F]);
            Renderer::getInstance()->font.draw(pzx, 116+16*3, "╚══════════════════════╝", VGA_COLORS[0x0F]);

            Renderer::getInstance()->font.draw(4, 0, "V", VGA_COLORS[0x04]);
            Renderer::getInstance()->font.draw(12, 0, "iXEn Helper v0.1", VGA_COLORS[0x15]);

            Renderer::getInstance()->font.draw(4, Renderer::getInstance()->screenHeight-16, "Y", VGA_COLORS[0x04]);
            Renderer::getInstance()->font.draw(12, Renderer::getInstance()->screenHeight-16, "ou can exit now", VGA_COLORS[0x15]);
        }

        void Finished::onKeyDown(Event::Keyboard* event)
        {
        }

        void Finished::onButtonDown(Event::Gamepad* event)
        {
        }

    }
}
