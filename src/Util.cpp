#include <cpr/cpr.h>
#include <taigabot/Util.hpp>

rapidjson::Document TaigaBot::Util::get_post(const char *url) {
	auto post_json_string =
		cpr::Get(cpr::Url{url}, cpr::Header{{"User-Agent", "taigabot-cpp"}})
			.text;

	rapidjson::Document post_json;
	post_json.Parse(post_json_string.c_str());

	while (post_json.IsNull() ||
		   post_json[0]["data"]["children"][0]["data"]["is_self"].GetBool()) {
		post_json = TaigaBot::Util::get_post(url);
	}

	return post_json;
}