#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <aegis/gateway/objects/message.hpp>
#include <deque>
#include <fifo_map.hpp>
#include <functional>
#include <iostream>
#include <taiga/Client.hpp>
#include <taiga/Config.hpp>

namespace Taiga::Command {
using Verb = std::function<void(aegis::gateway::events::message_create&,
								const std::deque<std::string>&,
								Taiga::Client&)>;

struct Parameter {
	std::string name;
	bool required = true;
};
struct Command {
	std::string name;
	std::string category;
	std::deque<Parameter> params;
	Verb verb;
	std::optional<std::string> description;
};

using MappedCommands = nlohmann::fifo_map<std::string, Command>;
using MappedCommand = MappedCommands::value_type;
extern MappedCommands all;

void add_command(Command command);
}  // namespace Taiga::Command

#endif
