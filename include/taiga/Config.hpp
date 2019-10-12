#pragma once

#include <optional>

namespace Taiga::Config {
struct Config {
	std::string name;
	std::string prefix;
	std::optional<std::string> currency_conv_api_key;
	std::optional<std::string> git_repo;
	std::optional<std::string> owner_id;
	std::optional<std::string> weather_api_key;
	int color;
};
}  // namespace Taiga::Config
