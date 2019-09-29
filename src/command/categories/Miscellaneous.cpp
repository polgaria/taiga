#include <aegis.hpp>
#include <taiga/command/Command.hpp>
#include <taiga/command/categories/Miscellaneous.hpp>
#include <taiga/util/String.hpp>
#include <taiga/util/Various.hpp>

Taiga::Command::Categories::Miscellaneous::Miscellaneous(
	const std::string& _name)
	: Taiga::Command::Category(_name) {}

COMMAND(rate) {
	const auto ratee = Taiga::Util::String::join(params, " ");

	const auto hash = std::hash<std::string>();
	std::mt19937 rand(hash(ratee));

	obj.channel.create_message(
		fmt::format("I'd rate {} a {}/10", ratee,
					std::uniform_int_distribution<>{0, 10}(rand)));
}

COMMAND(progress) {
	auto percent = Taiga::Util::Various::year_progress();
	std::string msg;

	for (auto progress = 0; progress < 20; progress++) {
		msg += progress < percent / 5 ? "▓" : "░";
	}

	obj.channel.create_message(fmt::format("{} {:.2f}%", msg, percent));
}

COMMAND(kva) {
	obj.channel.create_message(
		"ква ква ква  гав гав гав    мяяяяяу   беееее  муууу  ку ку");
}

COMMAND(_kill) {
	obj.channel.create_message("bye").then(
		[&](const auto&&) { client.get_bot().shutdown(); });
}

void Taiga::Command::Categories::Miscellaneous::init(spdlog::logger& log) {
	Taiga::Commands::add_command(
		Taiga::Commands::Command()
			.name("rate")
			.description("Rates things on a scale of 0 to 10.")
			.params({{"ratee"}})
			.function(rate)
			.category(*this),
		log);
	Taiga::Commands::add_command(
		Taiga::Commands::Command()
			.name("progress")
			.description("Progress to the end of the year.")
			.function(progress)
			.category(*this),
		log);
	// clang-format off
	Taiga::Commands::add_command(
		Taiga::Commands::Command()
			.name("kva")
			.function(kva)
			.category(*this),
		log);
	Taiga::Commands::add_command(
		Taiga::Commands::Command()
			.name("kill")
			.owner_only(true)
			.function(_kill)
			.category(*this),
		log);
	// clang-format on
}
