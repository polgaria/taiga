#include <date/date.h>
#include <date/tz.h>

#include <aisaka/util/String.hpp>
#include <taiga/Bot.hpp>
#include <taiga/categories/Since.hpp>
#include <taiga/util/Date.hpp>
#include <taiga/util/Math.hpp>

// util methods
static std::string string_progress(float percent, ushort length) {
	std::string msg;
	msg.reserve(length);

	for (uint8_t progress = 0; progress < length; progress++) {
		msg += progress < percent / 5 ? "▓" : "░";
	}

	return msg;
}

static std::tuple<float, float, float> minecraft_age() {
	const auto& mc_release_date = date::May / 17 / 2009;

	return Taiga::Util::Date::time_since_date(date::sys_days(mc_release_date));
}

std::pair<float, unsigned short> year_progress() {
	const auto& now = std::chrono::system_clock::now();

	const date::year_month_day& current_date = date::floor<date::days>(now);
	const auto& days_in_year = current_date.year().is_leap() ? 366 : 365;

	const auto& next_date =
		date::January / 1 / current_date.year() + date::years{1};
	const float days_until_next_year = static_cast<float>(
		(date::sys_days(next_date) - date::sys_days(current_date)).count());

	auto nonrounded_percent =
		100.f - (days_until_next_year / std::move(days_in_year) * 100);

	return std::make_pair(Taiga::Util::Math::round_to_dec_places(
							  std::move(nonrounded_percent), 2),
						  days_until_next_year);
}

// commands
static void _progress(const aegis::gateway::events::message_create& obj,
					  Taiga::Bot&, const std::deque<std::string>&,
					  const std::string&) {
	auto [percent, days_left] = year_progress();
	auto msg = string_progress(percent, 20);

	obj.channel.create_message(
		fmt::format("{} {:.2f}%, {} days left", msg, percent, days_left));
}

static void time_since(const aegis::gateway::events::message_create& obj,
					   Taiga::Bot&, const std::deque<std::string>& params,
					   const std::string&) {
	try {
		const auto& [days, months, years] =
			Taiga::Util::Date::time_since_date(params.front());
		obj.channel.create_message(fmt::format(
			"It has been {:.2f} days ({:.2f} months, {:.2f} years) since {}.",
			days, months, years, params.front()));
	} catch (const std::invalid_argument& err) {
		obj.channel.create_message(err.what());
	}
}

void Taiga::Categories::Since::init(spdlog::logger& log,
									Aisaka::Commands<Taiga::Bot>& commands) {
	using Command = Aisaka::Command<Taiga::Bot>;
	using Metadata = Aisaka::Metadata;

	commands.add_command(  //
		Command("progress")
			.aliases({"year"})
			.metadata(
				Metadata().description("Progress to the end of the year."))
			.function(_progress)
			.category(*this),
		log);

	commands.add_command(
		Command("time_since")
			.metadata(Metadata()
						  .description("The time since an ISO string.")
						  .examples(Examples{{"placeholder", "placeholder"}}))
			.aliases({"timesince", "ts"})
			.function(time_since)
			.category(*this),
		log);

	// clang-format off
	commands.add_command(
		Command("minecraft_age")
			.aliases({"mc", "minecraft"})
			.function([](const auto& obj, const auto&, const auto&,
						 const auto&) {
				const auto& [days, months, years] = minecraft_age();
				obj.channel.create_message(fmt::format(
					"Minecraft is {:.2f} days ({:.2f} months, {:.2f} years) old.",
					days, months, years));
			})
			.category(*this),
		log);
	// clang-format on		
}