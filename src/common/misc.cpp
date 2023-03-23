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

#include <cmath>
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctype.h>
#include <SDL.h>
#if defined(_WIN32)
#include <windows.h>
#endif

#include "basics.h"
#include "misc.h"

#if defined(_WIN32)
std::wstring widen(const std::string &str)
{
  // Convert an ASCII string to a Unicode String
  std::wstring wstrTo;
  wchar_t *wszTo    = new wchar_t[str.length() + 1];
  wszTo[str.size()] = L'\0';
  MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wszTo, (int)str.length());
  wstrTo = wszTo;
  delete[] wszTo;
  return wstrTo;
}
std::string narrow(const std::wstring &str)
{
  // Convert an ASCII string to a Unicode String
  std::string wstrTo;
  char *wszTo       = new char[str.length() + 1];
  wszTo[str.size()] = L'\0';
  WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, wszTo, (int)str.length(), NULL, NULL);
  wstrTo = wszTo;
  delete[] wszTo;
  return wstrTo;
}
#else
std::string widen(const std::string &str)
{
  return str;
}
std::string narrow(const std::string &str)
{
  return str;
}
#endif
