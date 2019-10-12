#pragma once

#include <aisaka/command/categories/Category.hpp>
#include <taiga/Client.hpp>

namespace Taiga::Categories {
class Prefix : public Aisaka::Category<Taiga::Client> {
   public:
	Prefix(const std::string& name) : Aisaka::Category<Taiga::Client>(name) {}
	virtual ~Prefix() = default;

	void init(spdlog::logger& log, Aisaka::Commands<Taiga::Client>& commands);
};
}  // namespace Taiga::Categories
