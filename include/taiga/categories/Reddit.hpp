#pragma once

#include <aisaka/command/categories/Category.hpp>
#include <taiga/Client.hpp>

namespace Taiga::Categories {
class Reddit : public Aisaka::Category<Taiga::Client> {
   public:
	Reddit(const std::string& name) : Aisaka::Category<Taiga::Client>(name) {}
	virtual ~Reddit() = default;

	void init(spdlog::logger& log, Aisaka::Commands<Taiga::Client>& commands);
};
}  // namespace Taiga::Categories
