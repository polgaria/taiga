#pragma once

#include <aisaka/command/categories/Category.hpp>
#include <taiga/Bot.hpp>

namespace Taiga::Categories {
class Weather : public Aisaka::Category<Taiga::Bot> {
   public:
	explicit Weather(const std::string_view name)
		: Aisaka::Category<Taiga::Bot>(name) {}

	void init(spdlog::logger& log,
			  Aisaka::Commands<Taiga::Bot>& commands) override;
};
}  // namespace Taiga::Categories
