#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <aegis.hpp>
#include <taigabot/Config.hpp>

#define COMMAND(name)                                 \
	void name(aegis::gateway::events::message_create, \
			  const std::deque<std::string>, TaigaBot::Client*);

namespace TaigaBot::Commands {
COMMAND(help)
COMMAND(taiga)
COMMAND(toradora)
COMMAND(progress)
COMMAND(money)
COMMAND(set_tz)
COMMAND(tz)

void add_commands(std::shared_ptr<spdlog::logger> log);
}  // namespace TaigaBot::Commands

#endif
