#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <aegis/gateway/objects/message.hpp>
#include <deque>
#include <functional>
#include <iostream>
#include <map>

namespace TaigaBot::Command {
using Verb = std::function<void(aegis::gateway::objects::message,
								std::deque<std::string>&)>;

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
