#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>

namespace TaigaBot::Config {
struct Config {
	std::string token;
	std::string currency_api_token;
};

Config load_config();
}  // namespace TaigaBot::Config

#endif