#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>

namespace Taiga::Config {
struct Config {
	std::string prefix;
	std::string currency_conv_api_key;
};
}  // namespace Taiga::Config

#endif