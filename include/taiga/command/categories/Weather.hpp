#pragma once

#include <taiga/command/categories/Category.hpp>

namespace Taiga::Categories {
class Weather : Taiga::Category {
   public:
	Weather(const std::string&);
	void init(spdlog::logger& log) override;
};
}  // namespace Taiga::Categories
