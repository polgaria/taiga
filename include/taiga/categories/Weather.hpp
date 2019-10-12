#pragma once

#include <aisaka/command/categories/Category.hpp>
#include <taiga/Client.hpp>

namespace Taiga::Categories {
class Weather : Aisaka::Category<Taiga::Client> {
   public:
	Weather(const std::string& name) : Aisaka::Category<Taiga::Client>(name) {}
	void init(spdlog::logger& log, Aisaka::Commands<Taiga::Client>& commands);
};
}  // namespace Taiga::Categories
