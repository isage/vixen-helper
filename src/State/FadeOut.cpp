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

#include "../State/FadeOut.h"

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
        FadeOut::FadeOut(State* state) : State(), _state(state)
        {
        }

        FadeOut::~FadeOut()
        {
        }

        void FadeOut::init()
        {
            if (_initialized) return;
            State::init();

            setModal(true);
            setFullscreen(false);
        }

        void FadeOut::think(uint32_t delta)
        {
            auto game = Game::Game::getInstance();
            State::think(delta);

            INC_PER_DELTA(_timer);

            if (_timer > 60.)
            {
                game->setState(_state);
            }
        }

        void FadeOut::render()
        {
            int x, y;
            int startframe = 0;

            for(y = Renderer::getInstance()->screenHeight - 32; y >= 0; y -= 32)
            {
                int f = 0;

                for(x = 0; x < Renderer::getInstance()->screenWidth; x+=32)
                {
                        int frame = (static_cast<int>(_timer) - startframe - f);

                        if (frame < 0) frame = 0;
                        if (frame > 15) frame = 15;

                        if (frame)
                            Renderer::getInstance()->sprites.drawSprite("Fade", x, y, frame);

                        f++;
                }

                startframe++;
            }
        }



    }
}
