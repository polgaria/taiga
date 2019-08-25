#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <taigabot/Client.hpp>

#define COMMAND(name)                                                    \
	void name(TaigaBot::Client& client, SleepyDiscord::Message& message, \
			  std::deque<std::string>& params);

namespace TaigaBot::Commands {
COMMAND(help)
COMMAND(taiga)
COMMAND(toradora)
COMMAND(progress)
COMMAND(money)

void add_commands();
}  // namespace TaigaBot::Commands

#endif
