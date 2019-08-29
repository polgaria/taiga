#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>

namespace TaigaBot::Config {
struct Config {
	std::string token;
	std::string prefix;
};

Config load_config();
Config get_config();
}  // namespace TaigaBot::Config

#endif