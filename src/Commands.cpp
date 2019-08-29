#include <date/date.h>
#include <date/tz.h>
#include <fmt/format.h>
#include <rapidjson/document.h>
#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>
#include <taigabot/Command.hpp>
#include <taigabot/Commands.hpp>
#include <taigabot/util/MathUtil.hpp>
#include <taigabot/util/StringUtil.hpp>
#include <taigabot/util/Util.hpp>

#ifdef COMMAND
#undef COMMAND
#define COMMAND(name)                                                       \
	void TaigaBot::Commands::name(aegis::gateway::objects::message message, \
								  std::deque<std::string>& params)
#endif

#define ADD_COMMAND(name, ...) \
	TaigaBot::Command::add_command({#name, __VA_ARGS__, name})

COMMAND(help) {
	constexpr char start[] = "```\\n";
	constexpr char the_end[] = "```";
	// estimate length
	auto length = strlen(start) + strlen(the_end);
	for (TaigaBot::Command::MappedCommand& command : TaigaBot::Command::all) {
		length += command.first.size();
		length += 2;  // ' ' and '\n'
		for (const auto& param : command.second.params) {
			length += 2;  // '<' / '[' and '> ' / '] '
			length += param.name.size();
		}
	}

	std::string output;
	output.reserve(length);
	output += start;
	for (TaigaBot::Command::MappedCommand& command : TaigaBot::Command::all) {
		output += command.first;
		output += ' ';
		for (const auto& param : command.second.params) {
			output += fmt::format(param.required ? "<{}>" : "[{}]", param.name);
		}
		output += "\\n";
	}
	output += the_end;
	message.get_channel().create_message(output);
}

COMMAND(taiga) {
	auto post =
		TaigaBot::Util::get_post("https://reddit.com/r/taiga/random.json");

	message.get_channel().create_message(
		post[0]["data"]["children"][0]["data"]["url"].GetString());
}

COMMAND(toradora) {
	auto post =
		TaigaBot::Util::get_post("https://reddit.com/r/toradora/random.json");

	message.get_channel().create_message(
		post[0]["data"]["children"][0]["data"]["url"].GetString());
}

COMMAND(progress) {
	auto percent = TaigaBot::Util::year_progress();
	std::string msg;

	for (auto progress = 0; progress < 20; progress++) {
		msg += progress < percent / 5 ? "▓" : "░";
	}

	message.get_channel().create_message(
		fmt::format("{} {:.2f}%", msg, percent));
}

COMMAND(money) {
	float conversion_rate;
	try {
		conversion_rate =
			TaigaBot::Util::conversion_rate(params.front(), params.at(1),
											"");  // TODO: config
	} catch (const std::runtime_error& error) {
		message.get_channel().create_message(error.what());
		return;
	}

	float amount;
	try {
		amount = TaigaBot::Util::String::string_to_number<float>(params.back());
	} catch (const std::runtime_error& error) {
		message.get_channel().create_message("Invalid arguments.");
		return;
	}

	auto worth = amount * conversion_rate;

	message.get_channel().create_message(
		fmt::format("{:.2f} {} is worth {:.2f} {}", amount,
					TaigaBot::Util::String::to_upper(params.front()), worth,
					TaigaBot::Util::String::to_upper(params.at(1))));
}

COMMAND(set_tz) {
	using bsoncxx::builder::stream::close_document;
	using bsoncxx::builder::stream::document;
	using bsoncxx::builder::stream::finalize;
	using bsoncxx::builder::stream::open_document;

	mongocxx::client mongodb_client{mongocxx::uri{}};

	auto db = mongodb_client["taigabot"];
	auto timezones = db["timezones"];

	auto timezone = params.front();
	date::zoned_time<std::chrono::nanoseconds, const date::time_zone*> time;
	try {
		time = date::zoned_time{timezone, std::chrono::system_clock::now()};
	} catch (const std::runtime_error& error) {
		message.get_channel().create_message("Invalid timezone.");
		return;
	}

	// clang-format off
	timezones.update_one(document{}
							<< "id" << message.author.id
							<< finalize,
						 document{}
						 	<< "$set" << open_document
								<< "timezone" << timezone
							<< close_document
							<< finalize,
						 mongocxx::options::update{}.upsert(true));
	// clang-format on
	message.get_channel().create_message(
		fmt::format("Your timezone is now set to {}.\\nYour time is: {}.",
					timezone, date::format("%F %H:%M", time)));
}

COMMAND(tz) {
	using bsoncxx::builder::stream::close_document;
	using bsoncxx::builder::stream::document;
	using bsoncxx::builder::stream::finalize;
	using bsoncxx::builder::stream::open_document;

	mongocxx::client mongodb_client{mongocxx::uri{}};

	auto db = mongodb_client["taigabot"];
	auto timezones = db["timezones"];

	auto server = message.get_channel().get_guild();
	if (params.size() != 0) {
		auto members = server.get_members();
		for (const auto& _member : members) {
		}
	}
	// const auto member_name =
	// member.nick == "" ? member.user.username : member.nick;

	const auto id =			// params.size() == 0
		message.author.id;  //: member.ID;
	bsoncxx::stdx::optional<bsoncxx::document::value> op_result =
		timezones.find_one(document{} << "id" << id << finalize);

	if (!op_result) {
		message.get_channel().create_message(
			params.size() == 0
				? "Your timezone has not been set."
				: fmt::format("{}'s timezone has not been set", ""));
		return;
	}

	auto result = op_result->view();

	auto timezone = result["timezone"].get_utf8().value.to_string();
	auto time = date::zoned_time{timezone, std::chrono::system_clock::now()};

	auto tz_message =
		params.size() == 0
			? fmt::format("Your timezone is {}.\\nYour time is: {}.", timezone,
						  date::format("%F %H:%M", time))
			: fmt::format("{0}'s timezone is {}.\\n{0}'s time is {}", "",
						  timezone, date::format("%F %H:%M", time));
	message.get_channel().create_message(tz_message);
}

void TaigaBot::Commands::add_commands() {
	ADD_COMMAND(help, {});
	ADD_COMMAND(taiga, {});
	ADD_COMMAND(toradora, {});
	ADD_COMMAND(progress, {});
	ADD_COMMAND(
		money,
		{{"currency to convert from"}, {"currency to convert to"}, {"amount"}});
	ADD_COMMAND(set_tz, {{"timezone"}});
	ADD_COMMAND(tz, {{"user", false}});
}