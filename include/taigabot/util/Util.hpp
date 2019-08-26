#ifndef UTIL_HPP
#define UTIL_HPP

#include <fmt/format.h>
#include <rapidjson/document.h>
#include <deque>

namespace TaigaBot::Util {
rapidjson::Document get_post(const char* url);
float year_progress();
float conversion_rate(const std::string& from,
					  const std::string& to,
					  const std::string& token);
}  // namespace TaigaBot::Util

#endif
