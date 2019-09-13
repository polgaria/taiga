#include <taiga/Command.hpp>

void Taiga::Command::add_command(Command command) {
	all.emplace(command.name, command);
}
Taiga::Command::MappedCommands Taiga::Command::all;