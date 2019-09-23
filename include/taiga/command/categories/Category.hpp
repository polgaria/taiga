#ifndef CATEGORIES_HPP
#define CATEGORIES_HPP

#include <spdlog/logger.h>
#include <iostream>

#define COMMAND(name)                                             \
	static void name(aegis::gateway::events::message_create& obj, \
					 const std::deque<std::string>& params,       \
					 Taiga::Client& client)

#define ADD_COMMAND(_name, ...)                                   \
	log.info(fmt::format("Adding command {}: {}", name, #_name)); \
	Taiga::Command::add_command({#_name, name, __VA_ARGS__, _name})

#define ADD_COMMAND_DESC(_name, desc, ...)                        \
	log.info(fmt::format("Adding command {}: {}", name, #_name)); \
	Taiga::Command::add_command({#_name, name, __VA_ARGS__, _name, desc})

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
