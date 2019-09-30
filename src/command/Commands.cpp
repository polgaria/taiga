#include <taiga/command/Commands.hpp>
#include <taiga/command/categories/Conversion.hpp>
#include <taiga/command/categories/General.hpp>
#include <taiga/command/categories/Miscellaneous.hpp>
#include <taiga/command/categories/Reddit.hpp>
#include <taiga/command/categories/Timezone.hpp>
#include <taiga/command/categories/Weather.hpp>

using Taiga::Commands;
using Command = Taiga::Commands::Command;
using Parameter = Taiga::Commands::Parameter;
using Metadata = Taiga::Commands::Metadata;

Parameter::Parameter(std::string_view _name) : _name(std::move(_name)) {}

Command::Command() {}
Command::Command(std::string _name) : _name(std::move(_name)) {}

#define GETTER_SETTER(fname, _class, type)                         \
	const type _class::fname() const noexcept { return _##fname; } \
	_class& _class::fname(const type fname) noexcept {             \
		_##fname = fname;                                          \
		return *this;                                              \
	}

#define GETTER_SETTER_ARG(fname, _class, type, param_type)         \
	const type _class::fname() const noexcept { return _##fname; } \
	_class& _class::fname(const param_type fname) noexcept {       \
		_##fname = fname;                                          \
		return *this;                                              \
	}

#define INIT_CATEGORY(category) Taiga::Categories::category{#category}.init(log)

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
	INIT_CATEGORY(Weather);
}

GETTER_SETTER(name, Command, std::string&)
GETTER_SETTER(category, Command, Taiga::Category&)
GETTER_SETTER(params, Command, std::deque<Commands::Parameter>&)
GETTER_SETTER(aliases, Command, std::unordered_set<std::string>&)
GETTER_SETTER(metadata, Command, Metadata&)
GETTER_SETTER(function, Command, Commands::Function&)
GETTER_SETTER(owner_only, Command, bool&)

GETTER_SETTER(required, Parameter, bool&)
GETTER_SETTER(name, Parameter, std::string_view&)

GETTER_SETTER(description, Metadata, std::string&)
using Examples = nlohmann::fifo_map<std::string, std::string>;
GETTER_SETTER_ARG(examples, Metadata, Examples&, Examples&&)

Taiga::Commands::MappedCommands Taiga::Commands::all;
std::unordered_map<std::string, Taiga::Category> Taiga::Commands::categories;