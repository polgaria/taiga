#pragma once

#include <taiga/command/categories/Category.hpp>

namespace Taiga::Command::Categories {
class Miscellaneous : Taiga::Command::Category {
   public:
	Miscellaneous(const std::string&);
	void init(spdlog::logger& log) override;
};
}  // namespace Taiga::Command::Categories
