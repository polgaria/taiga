#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <taigabot/Events.hpp>

#define COMMAND(name) \
	void name(aegis::gateway::objects::message, std::deque<std::string>&);

namespace TaigaBot::Commands {
COMMAND(help)
COMMAND(taiga)
COMMAND(toradora)
COMMAND(progress)
COMMAND(money)
COMMAND(set_tz)
COMMAND(tz)

void add_commands();
}  // namespace TaigaBot::Commands

#endif
