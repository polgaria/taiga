#ifndef UTIL_HPP
#define UTIL_HPP

#include <rapidjson/document.h>
#include <spdlog/fmt/bundled/format.h>
#include <deque>

namespace TaigaBot::Util {
nlohmann::json get_post(const char* url);
float year_progress();
float conversion_rate(const std::string& from,
					  const std::string& to,
					  const std::string& token);
}  // namespace TaigaBot::Util

#endif
