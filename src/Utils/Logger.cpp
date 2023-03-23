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

#include "Logger.h"
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <iostream>
#include <memory>
#include <string>

#include "../common/misc.h"

namespace VXHelper
{
namespace Utils
{
namespace Logger
{
  static const char *LOG_PATTERN = "%^[%H:%M:%S.%e] [%l] [%@ %!]: %v%$";
  std::vector<spdlog::sink_ptr> sinks;

  void init(std::string filename)
  {
    sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
#if defined(DEBUG)
    sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(widen(filename), true));
#endif
    spdlog::set_default_logger(std::make_shared<spdlog::logger>("VXHelper logger", begin(sinks), end(sinks)));

    spdlog::set_pattern(LOG_PATTERN);

    spdlog::set_error_handler([](const std::string &msg) {
        std::cerr << "spdlog error: " << msg << std::endl;
    });

    spdlog::flush_on(spdlog::level::debug);
#if defined(DEBUG)
    spdlog::set_level(spdlog::level::trace);
#else
    spdlog::set_level(spdlog::level::info);
#endif
  }


} // namespace Logger
} // namespace Utils
} // namespace VXHelper
