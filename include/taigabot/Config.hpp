#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>

namespace TaigaBot::Config {
struct Config {
	std::string prefix;
	std::string currency_conv_api_key;
};
}  // namespace TaigaBot::Config

#endif