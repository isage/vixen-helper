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
#include <iostream>
#include <stdexcept>
#include <string>

#include <sys/stat.h>

#include "ResourceManager.h"

#include <json.hpp>
#include <SDL.h>

namespace VXHelper
{

bool ResourceManager::fileExists(const std::string &filename)
{
  struct stat st;

  if (stat(filename.c_str(), &st) == 0)
  {
    return true;
  }
  return false;
}

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager() {}

ResourceManager *ResourceManager::getInstance()
{
  return Base::Singleton<ResourceManager>::get();
}

void ResourceManager::shutdown() {}

std::string ResourceManager::getLocalizedPath(const std::string &filename)
{
  std::vector<std::string> _paths;

#if defined(__VITA__)
  _paths.push_back("app0:/data/" + filename);
#endif

  _paths.push_back("data/" + filename);

  for (auto &_tryPath: _paths)
  {
    if (fileExists(_tryPath))
      return _tryPath;
  }

  return _paths.back();
}

std::string ResourceManager::getPrefPath(const std::string &filename)
{
  std::string _tryPath;

  char *prefpath = SDL_GetPrefPath(NULL, "VXHelper");
  _tryPath = std::string(prefpath) + std::string(filename);
  SDL_free(prefpath);

  return _tryPath;
}

std::string ResourceManager::getPathForDir(const std::string &dir)
{
  std::vector<std::string> _paths;

#if defined(__VITA__)
  _paths.push_back("app0:/data/" + dir);
#endif
  _paths.push_back("data/" + dir);


  for (auto &_tryPath: _paths)
  {
    if (fileExists(_tryPath))
      return _tryPath;
  }

  return _paths.back();
}

}

