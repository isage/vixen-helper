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

#ifndef GAME_GAME_H
#define GAME_GAME_H

#include <memory>
#include <string>
#include <vector>

#include "../Base/Singleton.h"

#include <SDL.h>

namespace VXHelper
{

namespace State
{
    class State;
}

namespace Event
{
    class Event;
    class Dispatcher;
}

namespace Game
{

const float FPS = 60.;
const float msPerFrame = 1000. / FPS;
#define PX_PER_DELTA() (float)(delta)/VXHelper::Game::msPerFrame
#define INC_PER_DELTA(x) x+=PX_PER_DELTA()

enum class Action {
    ACCEPT,
    DECLINE,
    FIRE,
    INVALID
};

static const std::string actionNames[4] = {
  "accept",
  "decline",
  "fire",
  "invalid"
};

class Game
{
public:
    static Game* getInstance();

    void shutdown();

    State::State* topState(unsigned offset = 0) const;
    void pushState(State::State* state);
    void setState(State::State* state);
    void popState(bool doDelete = true);

    static Action getActionFromName(std::string& name) {
        for(size_t i = 0; i != static_cast<size_t>(Action::INVALID); ++i) {
            if (name == actionNames[i]) {
                return static_cast<Action>(i);
            }
        }
        return Action::INVALID;
    }

    void run();
    void quit();
    void init();

    void handle();

    void think(uint32_t delta);

    void render();

    Event::Dispatcher* eventDispatcher();

protected:
    std::vector<std::unique_ptr<State::State>> _states;
    std::vector<std::unique_ptr<State::State>> _statesForDelete;
    std::unique_ptr<Event::Dispatcher> _eventDispatcher;

    bool _quit = false;
    bool _initialized = false;

    SDL_Event _event;

    std::vector<State::State*> _getVisibleStates();
    std::vector<State::State*> _getActiveStates();

private:
    friend class Base::Singleton<Game>;
    std::unique_ptr<Event::Event> _createEventFromSDL(const SDL_Event& sdlEvent);

    Game();
    ~Game();
    Game(Game const&) = delete;
    void operator=(Game const&) = delete;
    void _updateFps();

    uint32_t _lastTick;
    uint32_t _frames = 0;
    uint32_t _fps = 0;
    uint32_t _fpstimer = 0;

};


}
}


#endif // GAME_GAME_H
