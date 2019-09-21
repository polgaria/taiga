#include <date/date.h>
#include <date/tz.h>
#include <spdlog/spdlog.h>
#include <bsoncxx/builder/stream/document.hpp>
#include <fifo_map.hpp>
#include <functional>
#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>
#include <taiga/Client.hpp>
#include <taiga/Command.hpp>
#include <taiga/Commands.hpp>
#include <taiga/util/CommandUtil.hpp>
#include <taiga/util/MathUtil.hpp>
#include <taiga/util/StringUtil.hpp>
#include <taiga/util/Util.hpp>

// TODO: proper modules (because having everything in one file is fucking
// disgusting), current "categories" are just so the help is prettier and don't
// really do anything

#ifdef COMMAND
#undef COMMAND
#define COMMAND(name)                                                       \
	void Taiga::Commands::name(aegis::gateway::events::message_create& obj, \
							   const std::deque<std::string>& params,       \
							   Taiga::Client& client)
#endif

#define ADD_COMMAND(name, category, ...)               \
	log.info(fmt::format("Adding command {}", #name)); \
	Taiga::Command::add_command({#name, category, __VA_ARGS__, name})

#define ADD_COMMAND_DESC(name, desc, category, ...)    \
	log.info(fmt::format("Adding command {}", #name)); \
	Taiga::Command::add_command({#name, category, __VA_ARGS__, name, desc})

// General
COMMAND(help) {
	using aegis::gateway::objects::field;

	if (!params.empty()) {
		const auto& found_command = Taiga::Command::all.find(params.front());
		if (found_command == Taiga::Command::all.end()) {
			obj.channel.create_message("Command not found.");
			return;
		}

		const auto& command = Taiga::Command::all.at(params.front());
		auto fields = std::vector<field>();

		auto embed{aegis::gateway::objects::embed()
					   .title(fmt::format("**{}**", command.name))
					   .color(0x3498DB)};

		auto syntax{fmt::format(command.params.empty() ? "`{}{}" : "`{}{} ",
								client.get_config().prefix, command.name)};
		for (const auto& param : command.params) {
			syntax +=
				fmt::format(param.required ? "<{}> " : "[{}] ", param.name);
		}
		syntax += '`';

		fields.push_back({field().name("**Syntax**").value(syntax)});

		if (command.description) {
			fields.push_back({field()
								  .name("**Description**")
								  .value(command.description.value())});
		}

		embed.fields(fields);

		obj.channel.create_message(aegis::create_message_t().embed(embed));

		return;
	}

	auto embed{
		aegis::gateway::objects::embed().title("**Commands**").color(0x3498DB)};
	auto fields_content = nlohmann::fifo_map<std::string, std::string>();
	auto fields = std::vector<field>();

	for (const auto& command : Taiga::Command::all) {
		// should i be doing this?? there's probably a faster way.. eh whatever
		fields_content[command.second.category] +=
			fmt::format("`{}` ", command.first);
	}
	for (const auto& [category, content] : fields_content) {
		fields.push_back(field().name(category).value(content));
	}

	embed.fields(fields);
	obj.channel.create_message(aegis::create_message_t().embed(embed));
}

// Reddit
COMMAND(taiga) {
	obj.channel.create_message(
		Taiga::Util::get_random_reddit_post_url("taiga"));
}

COMMAND(toradora) {
	obj.channel.create_message(
		Taiga::Util::get_random_reddit_post_url("toradora"));
}

// Conversion
COMMAND(money) {
	const auto currency_x = Taiga::Util::String::to_upper(params.front());
	const auto currency_y = Taiga::Util::String::to_upper(params.at(1));

	// just.. no
	if (currency_x == currency_y) {
		obj.channel.create_message("no");

		return;
	}

	const auto _amount =
		params.size() >= 3
			? Taiga::Util::String::string_to_number<float>(params.at(2))
			: 1;
	if (!_amount) {
		obj.channel.create_message("Invalid parameters.");
	}
	const auto& amount = _amount.value();

	float conversion_rate;
	try {
		conversion_rate = Taiga::Util::conversion_rate(
			currency_x, currency_y, client.get_config().currency_conv_api_key,
			client.get_bot()->get_rest_controller());
	} catch (const std::runtime_error& error) {
		obj.channel.create_message(error.what());

		return;
	}

	auto worth = amount * conversion_rate;

	obj.channel.create_message(fmt::format(
		"{:.2f} {} is worth {:.2f} {}", amount, currency_x, worth, currency_y));
}

COMMAND(mbps) {
	const auto _value =
		!params.empty()
			? Taiga::Util::String::string_to_number<float>(params.front())
			: 1;
	if (!_value) {
		obj.channel.create_message("Invalid parameters.");
	}
	const auto& value = _value.value();

	obj.channel.create_message(
		fmt::format("{}Mb/s is {:.2f}Mbps", value, value * 8));
}

COMMAND(mbs) {
	const auto _value =
		!params.empty()
			? Taiga::Util::String::string_to_number<float>(params.front())
			: 1;
	if (!_value) {
		obj.channel.create_message("Invalid parameters.");
	}
	const auto& value = _value.value();

	obj.channel.create_message(
		fmt::format("{}Mbps is {:.2f}Mb/s", value, value / 8));
}

// Timezone
COMMAND(set_tz) {
	using bsoncxx::builder::stream::close_document;
	using bsoncxx::builder::stream::document;
	using bsoncxx::builder::stream::finalize;
	using bsoncxx::builder::stream::open_document;

	mongocxx::client mongodb_client{mongocxx::uri{}};

	auto db = mongodb_client["taiga"];
	auto timezones = db["timezones"];

	date::zoned_time<std::chrono::nanoseconds, const date::time_zone*> time;

	// i really don't wanna parse through the timezone DB.. it's 15-30x slower
	// than this
	auto split_timezone = Taiga::Util::String::split(params.front(), '/');
	std::string timezone;
	if (split_timezone.size() == 1) {
		timezone = Taiga::Util::String::to_upper(split_timezone.front());
	} else {
		for (auto& part : split_timezone) {
			part = Taiga::Util::String::to_lower(part);
			part[0] = std::towupper(static_cast<unsigned char>(part[0]));
		}
		timezone = Taiga::Util::String::join(split_timezone, "/");
	}

	try {
		time = date::zoned_time{timezone, std::chrono::system_clock::now()};
	} catch (const std::runtime_error& error) {
		obj.channel.create_message("Invalid timezone.");
		return;
	}

	// clang-format off
	timezones.update_one(document{}
							<< "id" << obj.msg.author.id
							<< finalize,
						 document{}
						 	<< "$set" << open_document
								<< "timezone" << timezone
							<< close_document
							<< finalize,
						 mongocxx::options::update{}.upsert(true));
	// clang-format on
	obj.channel.create_message(
		fmt::format("Your timezone is now set to "
					"{}.\nYour time is: {}.",
					timezone, date::format("%F %H:%M", time)));
}

COMMAND(tz) {
	using bsoncxx::builder::stream::close_document;
	using bsoncxx::builder::stream::document;
	using bsoncxx::builder::stream::finalize;
	using bsoncxx::builder::stream::open_document;

	mongocxx::client mongodb_client{mongocxx::uri{}};

	auto find_user = !params.empty();

	auto db = mongodb_client["taiga"];
	auto timezones = db["timezones"];

	std::string member_name{};
	auto id = !find_user ? obj.msg.author.id.get() : 0;

	const auto user_to_find = Taiga::Util::String::join(params, " ");

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
													 "been set",
													 member_name));
		return;
	}

	auto timezone = op_result->view()["timezone"].get_utf8().value.to_string();
	auto time = date::zoned_time{timezone, std::chrono::system_clock::now()};

	auto output = fmt::format(
		"Your timezone is "
		"{}.\nYour time is: {}.",
		timezone, date::format("%F %H:%M", time));
	if (find_user) {
		op_result = timezones.find_one(
			document{} << "id" << obj.msg.get_author_id().get() << finalize);
		if (op_result) {
			auto author_timezone =
				op_result->view()["timezone"].get_utf8().value.to_string();
			auto author_time = date::zoned_time{
				author_timezone, std::chrono::system_clock::now()};

			const auto time_difference_string = date::format(
				"%H", author_time.get_local_time() - time.get_local_time());
			// if there's even any difference
			if (time_difference_string != "-00") {
				// then we convert the difference to an integer
				const auto _time_difference =
					Taiga::Util::String::string_to_number<int>(
						time_difference_string);
				if (!_time_difference) {
					obj.channel.create_message("Something went wrong.");
					client.get_bot()->log->error(
						"Somehow failed to convert time difference to an "
						"integer.");

					return;
				}
				const auto time_difference = _time_difference.value();

				// are we behind or ahead?
				const auto is_behind = time_difference < 0;

				// stupid ancient spdlog version; it bundles an ancient version
				// of fmtlib as well, resulting in having to do this..
				output = fmt::format(
					"{0}'s timezone is "
					"{1}.\n{0}'s time is {2}.\nYou are {3} hours {4} them.",
					member_name, timezone, date::format("%F %H:%M", time),
					is_behind ? -time_difference : time_difference,
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
	}

	obj.channel.create_message(output);
}

// Miscellaneous
COMMAND(rate) {
	const auto ratee = Taiga::Util::String::join(params, " ");

	const auto hash = std::hash<std::string>();
	std::srand(hash(ratee));

	obj.channel.create_message(fmt::format(
		"I'd rate {} a {}/10", ratee, std::rand() / ((RAND_MAX + 1u) / 10)));
}

COMMAND(progress) {
	auto percent = Taiga::Util::year_progress();
	std::string msg;

	for (auto progress = 0; progress < 20; progress++) {
		msg += progress < percent / 5 ? "▓" : "░";
	}

	obj.channel.create_message(fmt::format("{} {:.2f}%", msg, percent));
}

void Taiga::Commands::add_commands(spdlog::logger& log) {
	ADD_COMMAND_DESC(help, "The command you're seeing right now.", "General",
					 {{"command", false}});

	ADD_COMMAND_DESC(taiga, "Sends a random image from r/taiga", "Reddit", {});
	ADD_COMMAND_DESC(toradora, "Sends a random image from r/toradora", "Reddit",
					 {});

	ADD_COMMAND(money, "Conversion",
				{{"currency to convert from"},
				 {"currency to convert to"},
				 {"amount", false}});
	ADD_COMMAND_DESC(mbps, "Converts Mb/s to Mbps.", "Conversion",
					 {{"value", false}});
	ADD_COMMAND_DESC(mbs, "Converts Mbps to Mb/s.", "Conversion",
					 {{"value", false}});

	ADD_COMMAND_DESC(set_tz, "Set your timezone.", "Timezone", {{"timezone"}});
	ADD_COMMAND_DESC(tz, "See your own or another user's timezone.", "Timezone",
					 {{"user", false}});

	ADD_COMMAND_DESC(rate, "Rates things on a scale of 0 to 10.",
					 "Miscellaneous", {{"thing to rate"}});
	ADD_COMMAND_DESC(progress, "Progress to the end of the year.",
					 "Miscellaneous", {});
}