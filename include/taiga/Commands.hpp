#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <aegis.hpp>
#include <taiga/Config.hpp>

#define COMMAND(name)                                  \
	void name(aegis::gateway::events::message_create&, \
			  const std::deque<std::string>&, Taiga::Client&);

namespace Taiga::Commands {
COMMAND(help)

COMMAND(taiga)
COMMAND(toradora)

COMMAND(money)
COMMAND(mbps)
COMMAND(mbs)

COMMAND(set_tz)
COMMAND(tz)

COMMAND(rate)
COMMAND(progress)

COMMAND(kva)

void add_commands(spdlog::logger& log);
}  // namespace Taiga::Commands

#endif
