#pragma once

#include <taiga/command/categories/Category.hpp>

namespace Taiga::Categories {
class Miscellaneous : Taiga::Category {
   public:
	Miscellaneous(const std::string&);
	void init(spdlog::logger& log) override;
};
}  // namespace Taiga::Categories
