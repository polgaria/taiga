#pragma once

#include <aisaka/command/categories/Category.hpp>
#include <taiga/Bot.hpp>

namespace Taiga::Categories {
class Reddit : public Aisaka::Category<Taiga::Bot> {
   public:
	explicit Reddit(const std::string& name)
		: Aisaka::Category<Taiga::Bot>(name) {}

	void init(spdlog::logger& log, Aisaka::Commands<Taiga::Bot>& commands);
};
}  // namespace Taiga::Categories
