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

#include "../Game/Game.h"

#include <algorithm>
#include <sstream>
#include <ctime>
#include <memory>

#include "../Utils/Logger.h"
#include "../Graphics/Renderer.h"
#include "../Settings.h"
#include "../State/State.h"
#include "../Event/Dispatcher.h"
#include "../Event/State.h"

namespace VXHelper
{
namespace Game
{

Game *Game::getInstance()
{
  return Base::Singleton<Game>::get();
}


Game::Game()
{
}

void Game::init()
{
    if (_initialized) return;
    (void)Settings::getInstance();

    Graphics::Renderer::getInstance()->init(1);
    Graphics::Renderer::getInstance()->showLoadingScreen();

    _eventDispatcher = std::make_unique<Event::Dispatcher>();

    srand(time(0));

    _initialized = true;
}

Game::~Game()
{
//    shutdown();
}

void Game::shutdown()
{
    while (!_states.empty()) popState();
    Graphics::Renderer::getInstance()->close();
    SDL_Quit();
}

void Game::pushState(State::State* state)
{
    _states.push_back(std::unique_ptr<State::State>(state));
    if (!state->initialized()) state->init();
    state->emitEvent(std::make_unique<Event::State>("push"), state->pushHandler());
    state->setActive(true);
    state->emitEvent(std::make_unique<Event::State>("activate"), state->activateHandler());
}

void Game::popState(bool doDelete)
{
    if (_states.size() == 0) return;

    State::State* state = _states.back().get();
    if (doDelete)
    {
        _statesForDelete.emplace_back(std::move(_states.back()));
    }
    else
    {
        _states.back().release();
    }
    _states.pop_back();
    state->setActive(false);
    state->emitEvent(std::make_unique<Event::State>("deactivate"), state->deactivateHandler());
    state->emitEvent(std::make_unique<Event::State>("pop"), state->popHandler());
}

void Game::setState(State::State* state)
{
    while (!_states.empty()) popState();
    pushState(state);
}

void Game::_updateFps()
{
  _frames++;

  if ((SDL_GetTicks() - _fpstimer) >= 500)
  {
    _fpstimer   = SDL_GetTicks();
    _fps        = _frames * 2;
    _frames = 0;
  }
}


void Game::run()
{
    LOG_INFO("Starting main loop");

    _lastTick = SDL_GetTicks();

    while (!_quit)
    {
        uint32_t delta = SDL_GetTicks() - _lastTick;
        _lastTick = SDL_GetTicks();

        handle();
        think(delta);
        render();

        _statesForDelete.clear();
        SDL_Delay(1);
    }
    LOG_INFO("Stopping main loop");
}

void Game::quit()
{
    _quit = true;
}


State::State* Game::topState(unsigned offset) const
{
    return (_states.rbegin() + offset)->get();
}

std::vector<State::State*> Game::_getVisibleStates()
{
    std::vector<State::State*> subset;
    if (_states.size() == 0)
    {
        return subset;
    }
    // we must render all states from last fullscreen state to the top of stack
    auto it = _states.end();
    do
    {
        --it;
    }
    while (it != _states.begin() && !(*it)->fullscreen());

    for (; it != _states.end(); ++it)
    {
        subset.push_back((*it).get());
    }
    return subset;
}

std::vector<State::State*> Game::_getActiveStates()
{
    // we must handle all states from top to bottom of stack
    std::vector<State::State*> subset;

    auto it = _states.rbegin();
    // active states
    for (; it != _states.rend(); ++it)
    {
        auto state = it->get();
        if (!state->active())
        {
            state->emitEvent(std::make_unique<Event::State>("activate"), state->activateHandler());
            state->setActive(true);
        }
        subset.push_back(state);
        if (state->modal() || state->fullscreen())
        {
            ++it;
            break;
        }
    }
    // not active states
    for (; it != _states.rend(); ++it)
    {
        auto state = it->get();
        if (state->active())
        {
            state->emitEvent(std::make_unique<Event::State>("deactivate"), state->activateHandler());
            state->setActive(false);
        }
    }
    return subset;
}

std::unique_ptr<Event::Event> Game::_createEventFromSDL(const SDL_Event& sdlEvent)
{
    using Keyboard = Event::Keyboard;
    using Gamepad = Event::Gamepad;
    using Text = Event::Text;
    switch (sdlEvent.type)
    {
        case SDL_KEYDOWN:
        {
            auto keyboardEvent = std::make_unique<Event::Keyboard>(Keyboard::Type::KEY_DOWN);
            keyboardEvent->setKeyCode(sdlEvent.key.keysym.sym);
            keyboardEvent->setAltPressed(sdlEvent.key.keysym.mod & KMOD_ALT);
            keyboardEvent->setShiftPressed(sdlEvent.key.keysym.mod & KMOD_SHIFT);
            keyboardEvent->setControlPressed(sdlEvent.key.keysym.mod & KMOD_CTRL);
            return std::move(keyboardEvent);
        }
        case SDL_KEYUP:
        {
            auto keyboardEvent = std::make_unique<Event::Keyboard>(Keyboard::Type::KEY_UP);
            keyboardEvent->setKeyCode(sdlEvent.key.keysym.sym);
            keyboardEvent->setAltPressed(sdlEvent.key.keysym.mod & KMOD_ALT);
            keyboardEvent->setShiftPressed(sdlEvent.key.keysym.mod & KMOD_SHIFT);
            keyboardEvent->setControlPressed(sdlEvent.key.keysym.mod & KMOD_CTRL);;

            return std::move(keyboardEvent);
        }
        case SDL_CONTROLLERBUTTONDOWN:
        {
            auto gamepadEvent = std::make_unique<Event::Gamepad>(Gamepad::Type::BUTTON_DOWN);
            gamepadEvent->setGamepad(sdlEvent.cbutton.which);
            gamepadEvent->setButton(sdlEvent.cbutton.button);
            return std::move(gamepadEvent);
        }
        case SDL_CONTROLLERBUTTONUP:
        {
            auto gamepadEvent = std::make_unique<Event::Gamepad>(Gamepad::Type::BUTTON_UP);
            gamepadEvent->setGamepad(sdlEvent.cbutton.which);
            gamepadEvent->setButton(sdlEvent.cbutton.button);
            return std::move(gamepadEvent);
        }
        case SDL_CONTROLLERAXISMOTION:
        {
            auto gamepadEvent = std::make_unique<Event::Gamepad>(Gamepad::Type::AXIS);
            gamepadEvent->setGamepad(sdlEvent.caxis.which);
            gamepadEvent->setAxis(sdlEvent.caxis.axis);
            gamepadEvent->setAxisValue(sdlEvent.caxis.value);
            return std::move(gamepadEvent);
        }

        case SDL_TEXTINPUT:
        {
            auto textEvent = std::make_unique<Event::Text>(Text::Type::INPUT);
            textEvent->setText(sdlEvent.text.text);
            return std::move(textEvent);
        }
        case SDL_TEXTEDITING:
        {
            auto textEvent = std::make_unique<Event::Text>(Text::Type::EDITING);
            textEvent->setText(sdlEvent.edit.text);
            return std::move(textEvent);
        }
    }
    return std::unique_ptr<Event::Event>();
}


void Game::handle()
{
    while (SDL_PollEvent(&_event))
    {
        if (_event.type == SDL_QUIT)
        {
            _quit = true;
        }
        else
        {
            auto event = _createEventFromSDL(_event);
            if (event)
            {
                for (auto state : _getActiveStates()) state->handle(event.get());
            }
        }
        _eventDispatcher->processScheduledEvents();
    }

}

void Game::think(uint32_t delta)
{
    for (auto state : _getVisibleStates())
    {
        state->think(delta);
    }
    _eventDispatcher->processScheduledEvents();
}

void Game::render()
{
    for (auto state : _getVisibleStates())
    {
        state->render();
    }

    if (Settings::getInstance()->displayFps())
    {
        _updateFps();

        std::string fpstext = fmt::format("{} fps", _fps);

        int x = (Graphics::Renderer::getInstance()->screenWidth - 4) - Graphics::Renderer::getInstance()->font.getWidth(fpstext);
        Graphics::Renderer::getInstance()->font.draw(x, 0, fpstext, 0xFF1F1F, true);

    }

    Graphics::Renderer::getInstance()->flip();
}

Event::Dispatcher* Game::eventDispatcher()
{
    return _eventDispatcher.get();
}


}
}
