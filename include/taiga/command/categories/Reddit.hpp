#pragma once

#include <taiga/command/categories/Category.hpp>

namespace Taiga::Categories {
class Reddit : public Taiga::Category {
   public:
	Reddit(const std::string&);
	void init(spdlog::logger& log) override;
};
}  // namespace Taiga::Categories
