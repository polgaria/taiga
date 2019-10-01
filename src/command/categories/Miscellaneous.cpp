#include <aegis/channel.hpp>
#include <aegis/gateway/events/message_create.hpp>
#include <taiga/command/Commands.hpp>
#include <taiga/command/categories/Miscellaneous.hpp>
#include <taiga/util/String.hpp>
#include <taiga/util/Various.hpp>

Taiga::Categories::Miscellaneous::Miscellaneous(const std::string& _name)
	: Taiga::Category(_name) {}

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

COMMAND(kva) {}

void Taiga::Categories::Miscellaneous::init(spdlog::logger& log) {
	using Command = Taiga::Commands::Command;
	using Metadata = Taiga::Commands::Metadata;
	using Parameter = Taiga::Commands::Parameter;

	Taiga::Commands::add_command(  //
		Command("rate")
			.metadata(
				Metadata().description("Rates things on a scale of 0 to 10."))
			.params({Parameter("ratee")})
			.function(rate)
			.category(*this),
		log);
	Taiga::Commands::add_command(  //
		Command("progress")
			.metadata(
				Metadata().description("Progress to the end of the year."))
			.function(progress)
			.category(*this),
		log);
	// clang-format off
	Taiga::Commands::add_command(
		Command("kva")
				.function([](const auto& obj, const auto&, const auto&, const auto&){
					obj.channel.create_message(
						"ква ква ква  гав гав гав    мяяяяяу   беееее  муууу  ку ку");
				})
			.aliases({"ква"})
			.category(*this),
		log);
	Taiga::Commands::add_command(
		Command("kill")
			.owner_only(true)
			.function(
				[](const auto& obj, const auto&, const auto&, auto& client) {
					obj.channel.create_message("bye").then(
						[&client](const auto&&) { client.get_bot().shutdown(); });
				})
			.category(*this),
		log);
	// clang-format on
}
