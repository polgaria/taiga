#include <aisaka/util/String.hpp>
#include <taiga/Bot.hpp>
#include <taiga/categories/Miscellaneous.hpp>

static void rate(const aegis::gateway::events::message_create& obj, Taiga::Bot&,
				 const std::deque<std::string>& params,
				 const std::string_view) {
	const auto ratee = Aisaka::Util::String::join(params, " ");

	const auto hash = std::hash<std::string_view>();
	std::mt19937 rand(hash(ratee));

	std::random_device rd;

	obj.channel.create_message(
		fmt::format("I'd rate {} a {}/10", ratee,
					std::uniform_int_distribution<>{0, 10}(rand)));
}

void Taiga::Categories::Miscellaneous::init(
	spdlog::logger& log, Aisaka::Commands<Taiga::Bot>& commands) {
	using Command = Aisaka::Command<Taiga::Bot>;
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
	// clang-format off
	commands.add_command(
		Command("kva")
			.function([](const auto& obj, const auto&, const auto&, const auto&) {
				obj.channel.create_message(
					"ква ква ква  гав гав гав    мяяяяяу   беееее  муууу  ку ку");
			})
			.aliases({"ква"})
			.category(*this),
		log);
	commands.add_command(
		Command("kill")
			.owner_only(true)
			.function([](const auto& obj, auto& client, const auto&, const auto&) {
				obj.channel.create_message("bye").then(
					[&client](const auto&&) { client.core().shutdown(); });
			})
			.category(*this),
		log);
	// clang-format on	
}
