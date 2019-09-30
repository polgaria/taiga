#pragma once

#include <taiga/command/categories/Category.hpp>

namespace Taiga::Categories {
class General : Taiga::Category {
   public:
	General(const std::string&);
	void init(spdlog::logger& log) override;
};
}  // namespace Taiga::Categories
