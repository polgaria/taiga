#pragma once

#include <taiga/command/categories/Category.hpp>

namespace Taiga::Categories {
class Timezone : Taiga::Category {
   public:
	Timezone(const std::string&);
	void init(spdlog::logger& log) override;
};
}  // namespace Taiga::Categories
