#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <taigabot/Client.hpp>
#include <taigabot/Util.hpp>

void TaigaBot::Client::onMessage(SleepyDiscord::Message message) {
	if (message.startsWith("!/taiga")) {
		auto post =
			TaigaBot::Util::get_post("https://reddit.com/r/taiga/random.json");

		sendMessage(message.channelID,
					post[0]["data"]["children"][0]["data"]["url"].GetString());
	} else if (message.startsWith("!/toradora")) {
		auto post = TaigaBot::Util::get_post(
			"https://reddit.com/r/toradora/random.json");

		sendMessage(message.channelID,
					post[0]["data"]["children"][0]["data"]["url"].GetString());
	}
}
