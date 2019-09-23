#include <taiga/command/Command.hpp>
#include <taiga/command/categories/Conversion.hpp>
#include <taiga/command/categories/General.hpp>
#include <taiga/command/categories/Miscellaneous.hpp>
#include <taiga/command/categories/Reddit.hpp>
#include <taiga/command/categories/Timezone.hpp>

#define INIT_CATEGORY(category) \
	Taiga::Command::Categories::category{#category}.init(log)

void Taiga::Command::add_command(Command command) {
	all.emplace(command.name, command);
}

void Taiga::Command::add_commands(spdlog::logger& log) {
	INIT_CATEGORY(General);
	INIT_CATEGORY(Reddit);
	INIT_CATEGORY(Conversion);
	INIT_CATEGORY(Timezone);
	INIT_CATEGORY(Miscellaneous);
}
Taiga::Command::MappedCommands Taiga::Command::all;