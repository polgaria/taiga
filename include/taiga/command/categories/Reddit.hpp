#pragma once

#include <taiga/command/categories/Category.hpp>

namespace Taiga::Command::Categories {
class Reddit : public Taiga::Command::Category {
   public:
	Reddit(const std::string&);

	void init(spdlog::logger& log) override;
};
}  // namespace Taiga::Command::Categories
