#ifndef UTIL_HPP
#define UTIL_HPP

#include <spdlog/fmt/bundled/format.h>
#include <aegis/rest/rest_controller.hpp>
#include <optional>

namespace Taiga::Util {
std::string get_random_reddit_post_url(const std::string& subreddit);
float year_progress();
float conversion_rate(const std::string& from,
					  const std::string& to,
					  const std::optional<std::string>& api_key,
					  aegis::rest::rest_controller& rc);
}  // namespace Taiga::Util

#endif
