#ifndef CATEGORIES_HPP
#define CATEGORIES_HPP

#include <spdlog/logger.h>
#include <iostream>

#define COMMAND(name)                                             \
	static void name(aegis::gateway::events::message_create& obj, \
					 const std::deque<std::string>& params,       \
					 Taiga::Client& client)

namespace Taiga::Command {
class Category {
   public:
	Category(const std::string& name);
	virtual ~Category();
	virtual void init(spdlog::logger&) {}

	std::string name;
};
}  // namespace Taiga::Command

#endif
