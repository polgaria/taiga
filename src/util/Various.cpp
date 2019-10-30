#include <cpr/cpr.h>
#include <date/date.h>

#include <aisaka/util/String.hpp>
#include <nlohmann/json.hpp>
#include <taiga/util/Math.hpp>
#include <taiga/util/String.hpp>
#include <taiga/util/Various.hpp>

std::string Taiga::Util::Various::get_random_reddit_post_url(
	const std::string_view subreddit) {
	auto post_json_string =
		// why not aegis::rest::rest_controller? it can't handle
		// redirects; therefore, cpr it is (as much as i don't want to use it)
		cpr::Get(cpr::Url{fmt::format("https://old.reddit.com/r/{}/random.json",
									  subreddit)},
				 cpr::Header{{"User-Agent", "taiga"}})
			.text;

	auto post_json = nlohmann::json::parse(std::move(post_json_string));

	while (post_json.is_null() ||
		   post_json[0]["data"]["children"][0]["data"]["is_self"].get<bool>()) {
		post_json = Taiga::Util::Various::get_random_reddit_post_url(subreddit);
	}

	return post_json[0]["data"]["children"][0]["data"]["url"]
		.get<std::string>();
}

float Taiga::Util::Various::conversion_rate(
	const std::string_view from, const std::string_view to,
	const std::optional<std::string>& api_key,
	aegis::rest::rest_controller& rc) {
	if (!api_key.has_value()) {
		throw std::runtime_error("Currency API key not set.");
	}
	auto currency_to_currency = fmt::format("{}_{}", from, to);
	Taiga::Util::String::to_upper(currency_to_currency);

	aegis::rest::request_params rp;
	rp.host = "free.currconv.com";
	rp.path = fmt::format("/api/v7/convert?q={}&compact=ultra&apiKey={}",
						  currency_to_currency, api_key.value());
	rp.method = aegis::rest::Get;
	rp.headers = {"User-Agent: taiga"};

	const auto& request =
		rc.execute2(std::forward<aegis::rest::request_params>(rp));

	switch (request.reply_code) {
		case aegis::rest::http_code::ok: {
			break;
		}
		case aegis::rest::http_code::too_many_requests: {
			throw std::runtime_error("Ratelimited.");
		}
		case aegis::rest::http_code::bad_request: {
			throw std::runtime_error("Invalid API key.");
		}
		case aegis::rest::http_code::service_unavailable:
		case aegis::rest::http_code::internal_server_error:
		case aegis::rest::http_code::server_down: {
			throw std::runtime_error(
				"Currency Conversion API is down. Please try again later.");
		}
		default: {
			throw std::runtime_error("Unknown error.");
		}
	}

	auto json = nlohmann::json::parse(std::move(request.content));

	if (json.find(currency_to_currency) == json.end()) {
		throw std::invalid_argument("Invalid parameters.");
	}
	return json[currency_to_currency].get<float>();
}

aegis::gateway::objects::embed Taiga::Util::Various::get_weather_embed(
	const std::string_view api_key, const std::string_view location) {
	using aegis::gateway::objects::field;
	using aegis::gateway::objects::footer;
	const auto request =
		cpr::Get(cpr::Url{fmt::format("http://api.weatherstack.com/"
									  "current?access_key={}&query={}",
									  api_key, location)},
				 cpr::Header{{"User-Agent", "taiga"}});

	switch (request.status_code) {
		case aegis::rest::http_code::ok: {
			break;
		}
		case aegis::rest::http_code::too_many_requests: {
			throw std::runtime_error("Ratelimited.");
		}
		case aegis::rest::http_code::service_unavailable:
		case aegis::rest::http_code::internal_server_error:
		case aegis::rest::http_code::server_down: {
			throw std::runtime_error(
				"Weather API is down. Please try again later.");
		}
		default: {
			throw std::runtime_error("Unknown error.");
		}
	}

	auto json = nlohmann::json::parse(std::move(request.text));

	if (json.find("request") == json.end()) {
		throw std::runtime_error("Invalid location.");
	}

	auto embed =
		aegis::gateway::objects::embed()
			.title(json["request"]["query"].get<std::string>())
			.color(json["current"]["is_day"].get<std::string>() == "yes"
					   ? 0xFDB813
					   : 0x0F1112);

	embed.description(
		fmt::format("*{}*", fmt::join(json["current"]["weather_descriptions"]
										  .get<std::vector<std::string>>(),
									  "\n")));

	const auto& temperature = json["current"]["temperature"].get<int>();
	const auto& feels_like = json["current"]["feelslike"].get<int>();
	const auto& time = json["location"]["localtime"].get<std::string>();
	const auto& humidity = json["current"]["humidity"].get<int>();
	const auto& wind_speed = json["current"]["wind_speed"].get<int>();
	const auto& wind_direction = json["current"]["wind_dir"].get<std::string>();
	const auto& precipation = json["current"]["precip"].get<float>();

	std::vector<field> fields = {
		field()
			.name("Temperature")
			.value(temperature != feels_like
					   ? fmt::format("{}°C\n*Feels like {}°C*",
									 std::move(temperature),
									 std::move(feels_like))
					   : fmt::format("{}°C", std::move(temperature))),
		field().name("Time").value(std::move(time)),
		field()
			.name("Humidity")
			.value(fmt::format("{}%", std::move(humidity)))};

	if (wind_speed != 0.f) {
		fields.push_back(
			field()
				.name("Wind Speed/Direction")
				.value(fmt::format("{}km/h | {}", std::move(wind_speed),
								   std::move(wind_direction))));
	}

	if (precipation != 0.f) {
		fields.push_back(
			field()
				.name("Precipation")
				.value(fmt::format("{}mm", std::move(precipation))));
	}

	embed.fields(std::move(fields));

	embed.thumbnail(
		aegis::gateway::objects::thumbnail(json["current"]["weather_icons"]
											   .get<std::vector<std::string>>()
											   .front()));

	embed.footer(
		aegis::gateway::objects::footer("Powered by https://weatherstack.com"));

	return embed;
}
