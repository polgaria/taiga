#pragma once

#include <taiga/command/categories/Category.hpp>

namespace Taiga::Categories {
class Conversion : Taiga::Category {
   public:
	Conversion(const std::string&);
	void init(spdlog::logger& log) override;
};
}  // namespace Taiga::Categories
