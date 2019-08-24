#include <cpr/cpr.h>
#include <date/date.h>
#include <math.h>
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

float TaigaBot::Util::year_progress() {
	auto now = std::chrono::system_clock::now();

	auto current_date = date::year_month_day{date::floor<date::days>(now)};
	auto days_in_year = current_date.year().is_leap() ? 366 : 365;

	auto next_date = date::year_month_day{
		date::January / 1 / current_date.year() + date::years{1}};

	auto days_until_next_year =
		(date::sys_days(next_date) - date::sys_days(current_date)).count();
	auto nonrounded_percent =
		100.f - (static_cast<float>(days_until_next_year) / days_in_year * 100);

	return std::round(nonrounded_percent * 100) / 100;
}