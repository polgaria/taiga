#pragma once

#include <spdlog/logger.h>
#include <fifo_map.hpp>
#include <iostream>

#define COMMAND(name)                                             \
	static void name(aegis::gateway::events::message_create& obj, \
					 const std::deque<std::string>& params,       \
					 const std::string& command_prefix, Taiga::Client& client)

namespace Taiga::Command {
class Category {
   public:
	Category(std::string name);
	virtual ~Category();
	virtual void init(spdlog::logger&) {}
	const std::string& get_name() const noexcept { return name; }

   protected:
	using Examples = nlohmann::fifo_map<std::string, std::string>;

   private:
	std::string name;
};
}  // namespace Taiga::Command
