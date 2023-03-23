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

#include "../State/Exit.h"

#include <ctime>
#include <string>
#include <vector>
#include <memory>

#include "../Game/Game.h"
#include "../Graphics/Renderer.h"
#include "../ResourceManager.h"
#include "../Settings.h"
#include "../Utils/Logger.h"


using namespace VXHelper::Graphics;

namespace VXHelper
{
    namespace State
    {
        Exit::Exit() : State()
        {
        }

        Exit::~Exit()
        {
        }

        void Exit::init()
        {
            if (_initialized) return;
            State::init();

            setModal(true);
            setFullscreen(false);
        }

        void Exit::think(uint32_t delta)
        {
            auto game = Game::Game::getInstance();
            State::think(delta);
            if (game->topState() == this)
                game->quit();
        }

        void Exit::onStateActivate(Event::State* event)
        {
//            LOG_DEBUG("activated");
        }

        void Exit::onStateDeactivate(Event::State* event)
        {
//            LOG_DEBUG("deactivated");
        }
    }
}
