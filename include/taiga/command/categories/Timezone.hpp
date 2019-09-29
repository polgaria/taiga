#pragma once

#include <taiga/command/categories/Category.hpp>

namespace Taiga::Command::Categories {
class Timezone : Taiga::Command::Category {
   public:
	Timezone(const std::string&);
	void init(spdlog::logger& log) override;
};
}  // namespace Taiga::Command::Categories
