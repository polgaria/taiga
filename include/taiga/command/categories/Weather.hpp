#pragma once

#include <taiga/command/categories/Category.hpp>

namespace Taiga::Command::Categories {
class Weather : Taiga::Command::Category {
   public:
	Weather(const std::string&);
	void init(spdlog::logger& log) override;
};
}  // namespace Taiga::Command::Categories
