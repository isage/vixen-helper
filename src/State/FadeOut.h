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

#ifndef STATE_FADEOUT_H
#define STATE_FADEOUT_H

#include "../State/State.h"

namespace VXHelper
{
    namespace State
    {
        class FadeOut : public State
        {
            public:
                FadeOut(State* state);
                ~FadeOut() override;

                void think(uint32_t delta) override;
                void init() override;
                void render() override;

            protected:
                float _timer = 0;
                State* _state;
        };
    }
}
#endif // STATE_FADEOUT_H
