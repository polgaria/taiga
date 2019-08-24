#include <fmt/format.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <taigabot/Client.hpp>
#include <taigabot/Util.hpp>

#define MATCH(command, s) command.startsWith(s)

void TaigaBot::Client::onMessage(SleepyDiscord::Message message) {
	if (MATCH(message, "!/taiga")) {
		auto post =
			TaigaBot::Util::get_post("https://reddit.com/r/taiga/random.json");

		sendMessage(message.channelID,
					post[0]["data"]["children"][0]["data"]["url"].GetString());
	} else if (MATCH(message, "!/toradora")) {
		auto post = TaigaBot::Util::get_post(
			"https://reddit.com/r/toradora/random.json");

		sendMessage(message.channelID,
					post[0]["data"]["children"][0]["data"]["url"].GetString());
	} else if (MATCH(message, "!/progress")) {
		auto percent = TaigaBot::Util::year_progress();
		std::string msg;

		for (auto progress = 0; progress < 20; progress++) {
            msg += progress < percent / 5 ? "▓" : "░";
		}

		sendMessage(message.channelID, fmt::format("{} {}%", msg, percent));
	}
}
