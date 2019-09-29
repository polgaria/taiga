#include <taiga/command/Command.hpp>
#include <taiga/command/categories/Conversion.hpp>
#include <taiga/command/categories/General.hpp>
#include <taiga/command/categories/Miscellaneous.hpp>
#include <taiga/command/categories/Reddit.hpp>
#include <taiga/command/categories/Timezone.hpp>

using Taiga::Commands;
using Command = Taiga::Commands::Command;

#define GETTER_SETTER(fname, type)                                  \
	const type Command::fname() const noexcept { return _##fname; } \
	Command& Command::fname(const type fname) noexcept {            \
		_##fname = fname;                                           \
		return *this;                                               \
	}

// stays incase it's ever needed again
#define GETTER_SETTER_ARG(fname, type, param_type)                  \
	const type Command::fname() const noexcept { return _##fname; } \
	Command& Command::fname(const param_type fname) noexcept {      \
		_##fname = fname;                                           \
		return *this;                                               \
	}

#define INIT_CATEGORY(category) \
	Taiga::Command::Categories::category{#category}.init(log)

void Commands::add_command(Taiga::Commands::Command command,
						   spdlog::logger& log) {
	if (categories.find(command.category().get_name()) == categories.end()) {
		categories.emplace(command.category().get_name(), command.category());
	}

	if (all.find(command.name()) != all.end()) {
		log.error("Tried to add duplicate command {}!", command.name());
		return;
	}
	log.info("Adding command {} (category {})", command.name(),
			 command.category().get_name());
	all.emplace(command.name(), command);
	if (!command.aliases().empty()) {
		for (const auto& alias : command.aliases()) {
			log.info("Adding alias {} for command {} (category {})", alias,
					 command.name(), command.category().get_name());
			all.emplace(alias, command);
		}
	}
}
void Commands::add_commands(spdlog::logger& log) {
	INIT_CATEGORY(General);
	INIT_CATEGORY(Reddit);
	INIT_CATEGORY(Conversion);
	INIT_CATEGORY(Timezone);
	INIT_CATEGORY(Miscellaneous);
}

GETTER_SETTER(name, std::string&)
GETTER_SETTER(category, Taiga::Command::Category&)
GETTER_SETTER(params, std::deque<Commands::Parameter>&)
GETTER_SETTER(aliases, std::unordered_set<std::string>&)
GETTER_SETTER(description, std::string&)
GETTER_SETTER(function, Commands::Function&)
GETTER_SETTER(owner_only, bool&)

Taiga::Commands::MappedCommands Taiga::Commands::all;
std::unordered_map<std::string, Taiga::Command::Category>
	Taiga::Commands::categories;