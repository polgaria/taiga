#pragma once

#include <aisaka/command/categories/Category.hpp>
#include <taiga/Client.hpp>

namespace Taiga::Categories {
class General : public Aisaka::Category<Taiga::Client> {
   public:
	General(const std::string& name) : Aisaka::Category<Taiga::Client>(name) {}
	virtual ~General() = default;

	void init(spdlog::logger& log, Aisaka::Commands<Taiga::Client>& commands);
};
}  // namespace Taiga::Categories
