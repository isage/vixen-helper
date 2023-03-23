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

#ifndef _BASICS_H
#define _BASICS_H

#include <cstdint>

#if defined(__APPLE__) || defined(_WIN32)
#define MAXPATHLEN 256
#else
#include <sys/param.h> // MAXPATHLEN
#endif

#ifndef PATH_MAX
#define PATH_MAX 259
#endif

#include "../Utils/Logger.h"

#define ASSERT(X)                                                                                                      \
  {                                                                                                                    \
    if (!(X))                                                                                                          \
    {                                                                                                                  \
      LOG_CRITICAL("** ASSERT FAILED: '%s' at %s(%d)", #X, __FILE__, __LINE__);                                        \
      exit(1);                                                                                                         \
    }                                                                                                                  \
  }

#define SWAP(A, B)                                                                                                     \
  {                                                                                                                    \
    A ^= B;                                                                                                            \
    B ^= A;                                                                                                            \
    A ^= B;                                                                                                            \
  }

#ifndef MIN
#define MIN(A, B) (((A) < (B)) ? (A) : (B))
#endif

#ifndef MAX
#define MAX(A, B) (((A) > (B)) ? (A) : (B))
#endif

#endif
