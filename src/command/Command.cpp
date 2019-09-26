#include <taiga/command/Command.hpp>
#include <taiga/command/categories/Conversion.hpp>
#include <taiga/command/categories/General.hpp>
#include <taiga/command/categories/Miscellaneous.hpp>
#include <taiga/command/categories/Reddit.hpp>
#include <taiga/command/categories/Timezone.hpp>

using Taiga::Commands;
using Command = Taiga::Commands::Command;

#define GETTER_SETTER(fname, type)                       \
	type Command::fname() noexcept { return _##fname; }  \
	Command& Command::fname(const type fname) noexcept { \
		_##fname = fname;                                \
		return *this;                                    \
	}

#define GETTER_SETTER_ARG(fname, type, param_type)             \
	type Command::fname() noexcept { return _##fname; }        \
	Command& Command::fname(const param_type fname) noexcept { \
		_##fname = fname;                                      \
		return *this;                                          \
	}

#define INIT_CATEGORY(category) \
	Taiga::Command::Categories::category{#category}.init(log)

void Commands::add_command(Taiga::Commands::Command command,
						   spdlog::logger& log) {
	if (all.find(command.name()) != all.end()) {
		log.error("Tried to add duplicate command {}!", command.name());
		return;
	}
	log.info("Adding command {} (category {})", command.name(),
			 command.category());
	all.emplace(command.name(), command);
}
void Commands::add_commands(spdlog::logger& log) {
	INIT_CATEGORY(General);
	INIT_CATEGORY(Reddit);
	INIT_CATEGORY(Conversion);
	INIT_CATEGORY(Timezone);
	INIT_CATEGORY(Miscellaneous);
}

GETTER_SETTER(name, std::string&)
GETTER_SETTER(category, std::string&)
GETTER_SETTER(params, std::deque<Commands::Parameter>&)
GETTER_SETTER_ARG(description, std::optional<std::string>, std::string&)
GETTER_SETTER(function, Commands::Function&)
GETTER_SETTER(owner_only, bool&)

Taiga::Commands::MappedCommands Taiga::Commands::all;