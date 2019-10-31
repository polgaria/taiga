#include <date/date.h>
#include <date/tz.h>

#include <aisaka/util/String.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>
#include <taiga/categories/Timezone.hpp>
#include <taiga/util/Command.hpp>
#include <taiga/util/String.hpp>

static void set_tz(aegis::gateway::events::message_create& obj,
				   Taiga::Bot& client, const std::deque<std::string>& params,
				   const std::string_view) {
	using bsoncxx::builder::stream::close_document;
	using bsoncxx::builder::stream::document;
	using bsoncxx::builder::stream::finalize;
	using bsoncxx::builder::stream::open_document;

	const auto& mongo_client = client.mongo_pool().acquire();
	const auto& tz_to_set = params.front();
	auto timezones = (*mongo_client)[client.config().name]["timezones"];

	using Time =
		date::zoned_time<std::chrono::nanoseconds, const date::time_zone*>;
	Time time;

	std::string_view timezone;
	for (const auto& tz : date::get_tzdb().zones) {
		const auto& tz_name = tz.name();
		if (Aisaka::Util::String::to_lower(tz_to_set) ==
			Aisaka::Util::String::to_lower(tz_name)) {
			timezone = tz_name;
			time = Time(std::move(&tz), std::chrono::system_clock::now());
			break;
		}
	}

	if (timezone.empty()) {
		obj.channel.create_message("Invalid timezone!");
		return;
	}

	// clang-format off
    timezones.update_one(document{}
                            << "id" << obj.msg.author.id
                            << finalize,
                         document{}
                            << "$set" << open_document
                                << "timezone" << timezone.data()
                            << close_document
                            << finalize,
                         mongocxx::options::update{}.upsert(true));
	// clang-format on
	obj.channel.create_message(
		fmt::format("Your timezone is now set to "
					"{}.\nYour time is: {}.",
					timezone, date::format("%F %H:%M", time)));
}

static void tz(aegis::gateway::events::message_create& obj, Taiga::Bot& client,
			   const std::deque<std::string>& params, const std::string_view) {
	using bsoncxx::builder::stream::close_document;
	using bsoncxx::builder::stream::document;
	using bsoncxx::builder::stream::finalize;
	using bsoncxx::builder::stream::open_document;

	auto find_user = !params.empty();

	const auto& mongo_client = client.mongo_pool().acquire();
	auto timezones = (*mongo_client)[client.config().name]["timezones"];

	std::string member_name;
	auto id = !find_user ? obj.msg.author.id.get() : 0;

	const auto& user_to_find = Aisaka::Util::String::join(params, " ");

	if (find_user) {
		const auto& _member =
			Taiga::Util::Command::find_user(user_to_find, obj.msg, client);

		if (!_member) {
			obj.channel.create_message("Could not find member!");
			return;
		}

		auto& member = _member.value().get();

		member_name = member.get_name(obj.channel.get_guild_id()) == ""
						  ? member.get_username()
						  : member.get_name(obj.channel.get_guild_id());
		id = member.get_id();
	}

	auto op_result = timezones.find_one(document{} << "id" << id << finalize);
	if (!op_result) {
		obj.channel.create_message(!find_user
									   ? "Your timezone has not "
										 "been set."
									   : fmt::format("{}'s timezone has not "
													 "been set.",
													 member_name));
		return;
	}

	const auto& timezone =
		op_result->view()["timezone"].get_utf8().value.to_string();
	const auto& now = std::chrono::system_clock::now();

	std::string output;
	if (find_user) {
		op_result = timezones.find_one(
			document{} << "id" << obj.msg.get_author_id().get() << finalize);
		if (op_result) {
			const auto& author_time = date::zoned_time(
				op_result->view()["timezone"].get_utf8().value.to_string(),
				now);
			const auto& time = date::zoned_time(timezone, now);

			auto time_difference_string = date::format(
				"%H", author_time.get_local_time() - time.get_local_time());
			// if there's even any difference
			if (time_difference_string != "-00") {
				// then we convert the difference to an integer
				const auto _time_difference =
					Taiga::Util::String::string_to_number<int>(
						time_difference_string);
				if (!_time_difference) {
					obj.channel.create_message("Something went wrong.");
					client.core().log->error(
						"Somehow failed to convert time difference to an "
						"integer.");

					return;
				}
				const auto& time_difference = _time_difference.value();

				// are we behind or ahead?
				const auto is_behind = time_difference < 0;

				// stupid ancient spdlog version; it bundles an ancient
				// version of fmtlib as well, resulting in having to do
				// this..
				output = fmt::format(
					"{0}'s timezone is {1}.\n"
					"Their time is {2}.\n"
					"You are {3} {4} {5} them.",
					member_name, timezone, date::format("%F %H:%M", time),
					std::abs(time_difference),
					std::abs(time_difference) != 1 ? "hours" : "hour",
					is_behind ? "behind" : "ahead of");
			} else {
				// no difference; we don't include the behind/ahead text
				// i also have to do the fmtlib bullshit for this to work
				// thanks aegis.cpp
				output = fmt::format(
					"{0}'s timezone is "
					"{1}.\n{0}'s time is {2}.",
					member_name, timezone, date::format("%F %H:%M", time));
			}
		}
	} else {
		const auto time = date::zoned_time(timezone, now);

		output = fmt::format(
			"Your timezone is "
			"{}.\nYour time is: {}.",
			timezone, date::format("%F %H:%M", time));
	}

	obj.channel.create_message(output);
}

void Taiga::Categories::Timezone::init(spdlog::logger& log,
									   Aisaka::Commands<Taiga::Bot>& commands) {
	using Command = Aisaka::Command<Taiga::Bot>;
	using Metadata = Aisaka::Metadata;
	using Parameter = Aisaka::Parameter;

	commands.add_command(  //
		Command("settz")
			.aliases({"set_tz", "mytimeis"})
			.category(*this)
			.metadata(Metadata().description("Set your timezone."))
			.params({Parameter("timezone")})
			.function(set_tz),
		log);
	commands.add_command(
		Command("tz")
			.aliases({"time", "timefor"})
			.category(*this)
			.metadata(Metadata()
						  .description("See your own or another user's time.")
						  .examples(Examples{
							  {"", "Sends your current time and timezone."},
							  {"Some User",
							   "Sends `Some User`'s current time, timezone "
							   "and the time difference between you and them, "
							   "if any."}}))
			.params({Parameter("user").required(false)})
			.function(tz),
		log);
}
