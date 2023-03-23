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

#ifndef __MISC_H_
#define __MISC_H_
#include <cstdint>
#include <cstdio>
#include <string>
#include <algorithm>
#include <cctype>
#include <locale>


#if defined(_WIN32)
std::wstring widen(const std::string &str);
std::string narrow(const std::wstring &str);
#define myfopen _wfopen
#else
std::string widen(const std::string &str);
std::string narrow(const std::string &str);
#define myfopen fopen
#endif

#endif