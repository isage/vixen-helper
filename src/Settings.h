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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

#include "Base/Singleton.h"

namespace VXHelper
{

class Settings
{
public:
    static Settings *getInstance();


    bool save();
    bool load();

    bool changed() const;
    bool displayFps() const;

    void apply();

protected:
    friend class Base::Singleton<Settings>;
    Settings();
    ~Settings();

    Settings(const Settings &) = delete;
    Settings &operator=(const Settings &) = delete;

private:
    bool _changed = true; // for startup

    bool _displayFps = false;
};

} // VXHelper

#endif // SETTINGS_H
