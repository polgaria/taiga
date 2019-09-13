#include <date/date.h>
#include <date/tz.h>
#include <rapidjson/document.h>
#include <spdlog/spdlog.h>
#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>
#include <taiga/Client.hpp>
#include <taiga/Command.hpp>
#include <taiga/Commands.hpp>
#include <taiga/util/CommandUtil.hpp>
#include <taiga/util/MathUtil.hpp>
#include <taiga/util/StringUtil.hpp>
#include <taiga/util/Util.hpp>

#ifdef COMMAND
#undef COMMAND
#define COMMAND(name)                                                      \
	void Taiga::Commands::name(aegis::gateway::events::message_create obj, \
							   const std::deque<std::string> params,       \
							   Taiga::Client* client)
#endif

#define ADD_COMMAND(name, ...)                          \
	log->info(fmt::format("Adding command {}", #name)); \
	Taiga::Command::add_command({#name, __VA_ARGS__, name})

COMMAND(help) {
	const auto start{"```\n"};
	const auto the_end{"```"};
	// estimate length
	auto length = strlen(start) + strlen(the_end);
	for (Taiga::Command::MappedCommand& command : Taiga::Command::all) {
		length += command.first.size();
		length += 2;  // ' ' and '\n'
		for (const auto& param : command.second.params) {
			length += 2;  // '<' / '[' and '>
						  // ' / '] '
			length += param.name.size();
		}
	}

	std::string output;
	output.reserve(length);
	output += start;
	for (Taiga::Command::MappedCommand& command : Taiga::Command::all) {
		output += command.first;
		output += ' ';
		for (const auto& param : command.second.params) {
			output +=
				fmt::format(param.required ? "<{}> " : "[{}] ", param.name);
		}
		output += "\n";
	}
	output += the_end;
	obj.channel.create_message(output);
}

COMMAND(taiga) {
	auto post = Taiga::Util::get_post(
		"https://reddit.com/r/taiga/"
		"random.json");

	obj.channel.create_message(
		post[0]["data"]["children"][0]["data"]["url"].get<std::string>());
}

COMMAND(toradora) {
	auto post = Taiga::Util::get_post(
		"https://reddit.com/r/toradora/"
		"random.json");

	obj.channel.create_message(
		post[0]["data"]["children"][0]["data"]["url"].get<std::string>());
}

COMMAND(progress) {
	auto percent = Taiga::Util::year_progress();
	std::string msg;

	for (auto progress = 0; progress < 20; progress++) {
		msg += progress < percent / 5 ? "▓" : "░";
	}

	obj.channel.create_message(fmt::format("{} {:.2f}%", msg, percent));
}

COMMAND(money) {
	const auto currency_x = Taiga::Util::String::to_upper(params.front());
	const auto currency_y = Taiga::Util::String::to_upper(params.at(1));

	float conversion_rate;
	try {
		conversion_rate = Taiga::Util::conversion_rate(
			currency_x, currency_y, client->get_config().currency_conv_api_key);
	} catch (const std::runtime_error& error) {
		obj.channel.create_message(error.what());
		client->get_bot()->log->error(error.what());

		return;
	}

	float amount;
	try {
		amount = Taiga::Util::String::string_to_number<float>(params.back());
	} catch (const std::runtime_error& error) {
		obj.channel.create_message("Invalid arguments.");

		return;
	}

	auto worth = amount * conversion_rate;

	obj.channel.create_message(fmt::format(
		"{:.2f} {} is worth {:.2f} {}", amount, currency_x, worth, currency_y));
}

COMMAND(set_tz) {
	using bsoncxx::builder::stream::close_document;
	using bsoncxx::builder::stream::document;
	using bsoncxx::builder::stream::finalize;
	using bsoncxx::builder::stream::open_document;

	mongocxx::client mongodb_client{mongocxx::uri{}};

	auto db = mongodb_client["taiga"];
	auto timezones = db["timezones"];

	auto timezone = params.front();
	date::zoned_time<std::chrono::nanoseconds, const date::time_zone*> time;
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

	auto find_user = params.size() != 0;

	auto db = mongodb_client["taiga"];
	auto timezones = db["timezones"];

	std::string member_name{};
	auto id = !find_user ? obj.msg.author.id.get() : 0;

	if (find_user) {
		try {
			const auto& member = Taiga::Util::Command::find_user(
				params.front(), obj.channel.get_guild_id(), client);
			member_name = member->get_name(obj.channel.get_guild_id()) == ""
							  ? member->get_username()
							  : member->get_name(obj.channel.get_guild_id());
			id = member->get_id();
		} catch (const std::runtime_error& error) {
			obj.channel.create_message(error.what());
		}
	}

	bsoncxx::stdx::optional<bsoncxx::document::value> op_result =
		timezones.find_one(document{} << "id" << id << finalize);
	if (!op_result) {
		obj.channel.create_message(!find_user
									   ? "Your timezone has not "
										 "been set."
									   : fmt::format("{}'s timezone has not "
													 "been set",
													 member_name));
		return;
	}

	auto result = op_result->view();

	auto timezone = result["timezone"].get_utf8().value.to_string();
	auto time = date::zoned_time{timezone, std::chrono::system_clock::now()};

	obj.channel.create_message(
		!find_user ? fmt::format("Your timezone is "
								 "{}.\nYour time is: {}.",
								 timezone, date::format("%F %H:%M", time))
				   : fmt::format("{0}'s timezone is "
								 "{1}.\n{0}'s time is {2}.",
								 member_name, timezone,
								 date::format("%F %H:%M", time)));
}

void Taiga::Commands::add_commands(std::shared_ptr<spdlog::logger> log) {
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