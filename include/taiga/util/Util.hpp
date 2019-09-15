#ifndef UTIL_HPP
#define UTIL_HPP

#include <spdlog/fmt/bundled/format.h>

namespace Taiga::Util {
std::string get_random_reddit_post_url(const std::string& subreddit);
float year_progress();
float conversion_rate(const std::string& from,
					  const std::string& to,
					  const std::string& token,
					  aegis::rest::rest_controller& rc);
}  // namespace Taiga::Util

#endif
