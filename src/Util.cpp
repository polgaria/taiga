#include <cpr/cpr.h>
#include <taigabot/Util.hpp>

rapidjson::Document TaigaBot::Util::get_post(const char *url) {
	auto post_json_string =
		cpr::Get(cpr::Url{"https://old.reddit.com/r/Taiga/random.json"},
				 cpr::Header{{"User-Agent", "taigabot-cpp"}})
			.text;

	rapidjson::Document post_json;
	post_json.Parse(post_json_string.c_str());

	return post_json;
}