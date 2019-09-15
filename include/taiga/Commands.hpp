#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <aegis.hpp>
#include <taiga/Config.hpp>

#define COMMAND(name)                                  \
	void name(aegis::gateway::events::message_create&, \
			  const std::deque<std::string>&, Taiga::Client&);

namespace Taiga::Commands {
COMMAND(help)
COMMAND(progress)

COMMAND(taiga)
COMMAND(toradora)

COMMAND(money)

COMMAND(set_tz)
COMMAND(tz)

COMMAND(rate)

void add_commands(spdlog::logger& log);
}  // namespace Taiga::Commands

#endif
