#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <iostream>
#include <taigabot/Client.hpp>

namespace TaigaBot::Command {
using Verb = std::function<
	void(TaigaBot::Client&, SleepyDiscord::Message&, std::deque<std::string>&)>;
struct Command {
	std::string name;
	std::deque<std::string> params;
	Verb verb;
};

using MappedCommands = std::unordered_map<std::string, Command>;
using MappedCommand = MappedCommands::value_type;
extern MappedCommands all;

void add_command(Command command);
}  // namespace TaigaBot::Command

#endif
