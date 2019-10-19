#include <aisaka/util/String.hpp>
#include <taiga/Client.hpp>
#include <taiga/categories/Miscellaneous.hpp>
#include <taiga/util/Various.hpp>

static void rate(aegis::gateway::events::message_create& obj, Taiga::Client&,
				 const std::deque<std::string_view>& params,
				 const std::string&) {
	const auto ratee = Aisaka::Util::String::join(params, " ");

	const auto hash = std::hash<std::string_view>();
	std::mt19937 rand(hash(ratee));

	obj.channel.create_message(
		fmt::format("I'd rate {} a {}/10", ratee,
					std::uniform_int_distribution<>{0, 10}(rand)));
}

static void progress(aegis::gateway::events::message_create& obj,
					 Taiga::Client&, const std::deque<std::string_view>&,
					 const std::string&) {
	auto percent = Taiga::Util::Various::year_progress();
	std::string msg;

	for (auto progress = 0; progress < 20; progress++) {
		msg += progress < percent / 5 ? "▓" : "░";
	}

	obj.channel.create_message(fmt::format("{} {:.2f}%", msg, percent));
}

void Taiga::Categories::Miscellaneous::init(
	spdlog::logger& log, Aisaka::Commands<Taiga::Client>& commands) {
	using Command = Aisaka::Command<Taiga::Client>;
	using Metadata = Aisaka::Metadata;
	using Parameter = Aisaka::Parameter;

	commands.add_command(  //
		Command("rate")
			.metadata(
				Metadata().description("Rates things on a scale of 0 to 10."))
			.params({Parameter("ratee")})
			.function(rate)
			.category(*this),
		log);
	commands.add_command(  //
		Command("progress")
			.metadata(
				Metadata().description("Progress to the end of the year."))
			.function(progress)
			.category(*this),
		log);
	// clang-format off
	commands.add_command(
		Command("kva")
				.function([](const auto& obj, const auto&, const auto&, const auto&){
					obj.channel.create_message(
						"ква ква ква  гав гав гав    мяяяяяу   беееее  муууу  ку ку");
				})
			.aliases({"ква"})
			.category(*this),
		log);
	commands.add_command(
		Command("kill")
			.owner_only(true)
			.function(
				[](const auto& obj, auto& client, const auto&, const auto&) {
					obj.channel.create_message("bye").then(
						[&client](const auto&&) { client.bot().shutdown(); });
				})
			.category(*this),
		log);
	// clang-format on
}
