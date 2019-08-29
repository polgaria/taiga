#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <iostream>
#include <taigabot/Client.hpp>

namespace TaigaBot::Command {
using Verb = std::function<
	void(TaigaBot::Client&, SleepyDiscord::Message&, std::deque<std::string>&)>;

struct Parameter {
	std::string name;
	bool required = true;
};
struct Command {
	std::string name;
	std::deque<Parameter> params;
	Verb verb;
};

using MappedCommands = std::unordered_map<std::string, Command>;
using MappedCommand = MappedCommands::value_type;
extern MappedCommands all;

void add_command(Command command);
}  // namespace TaigaBot::Command

#endif
