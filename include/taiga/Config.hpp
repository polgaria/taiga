#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <optional>

namespace Taiga::Config {
struct Config {
	std::string name;
	std::string prefix;
	std::optional<std::string> currency_conv_api_key;
	std::optional<std::string> git_repo;
};
}  // namespace Taiga::Config

#endif
