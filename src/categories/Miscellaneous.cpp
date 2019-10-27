#include <aisaka/util/String.hpp>
#include <taiga/Bot.hpp>
#include <taiga/categories/Miscellaneous.hpp>
#include <taiga/util/Various.hpp>

static std::string string_progress(float percent, ushort length) {
	std::string msg;
	msg.reserve(length);

	for (uint8_t progress = 0; progress < length; progress++) {
		msg += progress < percent / 5 ? "▓" : "░";
	}

	return msg;
}

static void rate(const aegis::gateway::events::message_create& obj, Taiga::Bot&,
				 const std::deque<std::string>& params, const std::string&) {
	const auto ratee = Aisaka::Util::String::join(params, " ");

	const auto hash = std::hash<std::string_view>();
	std::mt19937 rand(hash(ratee));

	std::random_device rd;
	std::mt19937 guy(rd());
	std::cout << static_cast<wchar_t>(std::uniform_real_distribution<>{
		0, std::numeric_limits<double>::max()}(rand));

	obj.channel.create_message(
		fmt::format("I'd rate {} a {}/10", ratee,
					std::uniform_int_distribution<>{0, 10}(rand)));
}

static void _progress(const aegis::gateway::events::message_create& obj,
					  Taiga::Bot&, const std::deque<std::string>&,
					  const std::string&) {
	auto [percent, days_left] = Taiga::Util::Various::year_progress();
	auto msg = string_progress(percent, 20);

	obj.channel.create_message(
		fmt::format("{} {:.2f}%, {} days left", msg, percent, days_left));
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
	commands.add_command(  //
		Command("progress")
			.aliases({"year"})
			.metadata(
				Metadata().description("Progress to the end of the year."))
			.function(_progress)
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
	commands.add_command(
		Command("minecraft_age")
			.aliases({"mc", "minecraft"})				
			.function([](const auto& obj, const auto&, const auto&, const auto&) {
				obj.channel.create_message(fmt::format("Minecraft is {} days old.", Taiga::Util::Various::minecraft_age()));
			})
			.category(*this),
		log);
	// clang-format on	
}
