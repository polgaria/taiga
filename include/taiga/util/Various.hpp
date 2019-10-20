#pragma once

#include <aegis.hpp>
#include <optional>

namespace Taiga::Util {
class Various {
   public:
	static std::string get_random_reddit_post_url(const std::string& subreddit);
	static std::pair<float, unsigned short> year_progress();
	static float conversion_rate(const std::string& from, const std::string& to,
								 const std::optional<std::string>& api_key,
								 aegis::rest::rest_controller& rc);
	static aegis::gateway::objects::embed get_weather_embed(
		const std::string& api_key, const std::string& location);
};
}  // namespace Taiga::Util
