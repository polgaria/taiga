#include <taigabot/Command.hpp>

void TaigaBot::Command::add_command(Command command) {
	all.emplace(command.name, command);
}
TaigaBot::Command::MappedCommands TaigaBot::Command::all;