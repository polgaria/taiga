#pragma once

#include <aegis.hpp>
#include <optional>

namespace Taiga::Util {
class Various {
   public:
	static std::string get_random_reddit_post_url(
		const std::string_view subreddit);
	static std::pair<float, unsigned short> year_progress();
	static std::tuple<unsigned short, float, float> minecraft_age();
	static float conversion_rate(const std::string_view from,
								 const std::string_view to,
								 const std::optional<std::string>& api_key,
								 aegis::rest::rest_controller& rc);
	static aegis::gateway::objects::embed get_weather_embed(
		const std::string_view api_key, const std::string_view location);
};
}  // namespace Taiga::Util
