#pragma once

#include <taiga/command/categories/Category.hpp>

namespace Taiga::Command::Categories {
class Conversion : Taiga::Command::Category {
   public:
	Conversion(const std::string&);
	void init(spdlog::logger& log) override;
};
}  // namespace Taiga::Command::Categories
