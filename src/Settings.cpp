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

#include <fstream>
#include <string>
#include <stdexcept>

#include "ResourceManager.h"
#include "Utils/Logger.h"
#include "Settings.h"

namespace VXHelper
{

Settings *Settings::getInstance()
{
  return Base::Singleton<Settings>::get();
}


Settings::Settings()
{
    if (!load())
    {
        save();
    }
}

Settings::~Settings()
{
}

bool Settings::save()
{
    return true;
}


bool Settings::load()
{
    return true;
}

void Settings::apply()
{
}

bool Settings::changed() const
{
    return _changed;
}

bool Settings::displayFps() const
{
    return _displayFps;
}

}
