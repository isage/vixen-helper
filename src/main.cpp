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

#include <memory>
#include <SDL.h>

#include "Game/Game.h"
#include "Utils/Logger.h"
#include "Settings.h"
#include "ResourceManager.h"
#include "State/Start.h"

using namespace VXHelper;

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    Utils::Logger::init(ResourceManager::getInstance()->getPrefPath("debug.log"));

    SDL_GameController *controller = NULL;
    for (int i = 0; i < SDL_NumJoysticks(); ++i)
    {
        if (SDL_IsGameController(i))
        {
            controller = SDL_GameControllerOpen(i);
            if (controller)
            {
                  LOG_INFO("Opened controller {}", i);
                  LOG_INFO("Name: {}", SDL_GameControllerName(controller));
                  LOG_INFO("Mapping: {}", SDL_GameControllerMapping(controller));
            }
        }
    }

    auto game = Game::Game::getInstance();
    game->init();

    game->setState(new State::Start());

    game->run();
    game->shutdown();
    return 0;
}
