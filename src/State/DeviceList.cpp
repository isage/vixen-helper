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

#include "../State/DeviceList.h"

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
#include "../State/DeviceReading.h"

#if defined(__VITA__)
#include <psp2common/types.h>
#include <psp2/usbd.h>
#endif

using namespace VXHelper::Graphics;

namespace VXHelper
{
    namespace State
    {
        DeviceList::DeviceList() : State()
        {
        }

        DeviceList::~DeviceList()
        {
        }

        void DeviceList::init()
        {
            if (_initialized) return;
            State::init();

            setModal(true);
            setFullscreen(true);
#if defined(__VITA__)
            SceUID uid;
            SceUsbdDeviceInfo info[8] = {0};
            sceUsbdInit(&uid);
            sceUsbdGetDeviceList(uid, 8, (SceUsbdDeviceInfo *)&info);
            
            int i = 0;
            while (i < 8 && info[i].device_num != 0xffffffff && info[i].device_num > 0) {
                printf("devnum: 0x%08x\n", info[i].device_num);
                unsigned int device_id = (info[i].port << 16) + info[i].device_num;
                int size = sceUsbdGetDescriptorSize(uid, device_id);
                uint8_t* descr = (uint8_t*)malloc(size);
                if (descr)
                {
                    sceUsbdGetDescriptor(uid, device_id,  descr, size);
                    SceUsbdDeviceDescriptor *d = (SceUsbdDeviceDescriptor*)descr;
                    printf("VID: 0x%04x, PID: 0x%04x\n", d->idVendor, d->idProduct);
                    _list.push_back({d->idVendor, d->idProduct});
                    free(descr);
                }
                i++;
            }
            sceUsbdEnd(uid);
#endif
        }

        void DeviceList::think(uint32_t delta)
        {
            State::think(delta);
        }

        void DeviceList::render()
        {
            Renderer::getInstance()->clearScreen(VGA_COLORS[0x08]);
            Renderer::getInstance()->fillRect(0, 0, Renderer::getInstance()->screenWidth, 16, VGA_COLORS[0x1F]);

            Renderer::getInstance()->fillRect(0, Renderer::getInstance()->screenHeight-16, Renderer::getInstance()->screenWidth, Renderer::getInstance()->screenHeight, VGA_COLORS[0x1F]);

            int pzx = (Renderer::getInstance()->screenWidth / 2) - (212/2);

            Renderer::getInstance()->fillRect(pzx, 116, pzx+270, 116+64+(16*_list.size()), VGA_COLORS[1]);
            Renderer::getInstance()->font.draw(pzx, 116+16*0,         "╔═══════Select device════════╗", VGA_COLORS[0x0F]);
            Renderer::getInstance()->font.draw(pzx, 116+16*1,         "║                            ║", VGA_COLORS[0x0F]);
            int i;
            for(i = 0; i < _list.size(); i++)
            {
                char buf[256];
                snprintf(buf, 256, "║       0x%04x:0x%04x        ║", _list[i].vid, _list[i].pid);
                Renderer::getInstance()->font.draw(pzx, 116+16*(i+2), buf, VGA_COLORS[0x0F]);
            }
            Renderer::getInstance()->font.draw(pzx, 116+16*(i+2),     "║                            ║", VGA_COLORS[0x0F]);
            Renderer::getInstance()->font.draw(pzx, 116+16*(i+3),     "╚════════════════════════════╝", VGA_COLORS[0x0F]);

            Renderer::getInstance()->font.draw(pzx+56, 116+16*(_menuPos+2), "\x10", VGA_COLORS[0x0F]);

            Renderer::getInstance()->font.draw(4, 0, "V", VGA_COLORS[0x04]);
            Renderer::getInstance()->font.draw(12, 0, "iXEn Helper v0.1", VGA_COLORS[0x15]);

            Renderer::getInstance()->font.draw(4, Renderer::getInstance()->screenHeight-16, "S", VGA_COLORS[0x04]);
            Renderer::getInstance()->font.draw(12, Renderer::getInstance()->screenHeight-16, "elect device and press X...", VGA_COLORS[0x15]);
        }

        void DeviceList::onKeyDown(Event::Keyboard* event)
        {
            auto game = Game::Game::getInstance();

            switch (event->keyCode())
            {
                case SDLK_z:
                    game->pushState(new FadeOut(new Start()));
                    break;
                case SDLK_x:
                    game->pushState(new FadeOut(new DeviceReading(0,0)));
                    break;
                default:
                  break;
            }
        }

        void DeviceList::onButtonDown(Event::Gamepad* event)
        {
            auto game = Game::Game::getInstance();

            switch (event->button())
            {
                case SDL_CONTROLLER_BUTTON_B:
                    game->pushState(new FadeOut(new Start()));
                    break;
                case SDL_CONTROLLER_BUTTON_A:
                    game->pushState(new FadeOut(new DeviceReading(_list[_menuPos].vid, _list[_menuPos].pid)));
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                    _menuPos++;
                    if (_menuPos >= _list.size()) _menuPos = 0;
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_UP:
                    _menuPos--;
                    if (_menuPos <0 ) _menuPos = _list.size()-1;
                    break;

                default:
                  break;
            }
        }

    }
}
