#include <cpr/cpr.h>

#include <aisaka/util/String.hpp>
#include <taiga/Bot.hpp>
#include <taiga/categories/Weather.hpp>
#include <taiga/util/Various.hpp>

static aegis::gateway::objects::embed get_weather_embed(
	const std::string_view api_key, const std::string_view location) {
	using aegis::gateway::objects::field;
	using aegis::gateway::objects::footer;

	if (api_key.empty()) {
		throw std::invalid_argument("Weatherstack API key not set!");
	}

	const auto& request =
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
		throw std::invalid_argument("Invalid location.");
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

static void weather(aegis::gateway::events::message_create& obj,
					Taiga::Bot& client, const std::deque<std::string>& params,
					const std::string_view) {
	if (!client.config().weather_api_key) {
		obj.channel.create_message("Weatherstack API key not set!");
		return;
	}

	const auto& location = Aisaka::Util::String::join(params, "%20");
	try {
		obj.channel.create_message_embed(
			aegis::create_message_t().embed(get_weather_embed(
				client.config().weather_api_key.value(), location)));
	} catch (const std::exception& error) {
		obj.channel.create_message(error.what());
		return;
	}
}

void Taiga::Categories::Weather::init(spdlog::logger& log,
									  Aisaka::Commands<Taiga::Bot>& commands) {
	using Command = Aisaka::Command<Taiga::Bot>;
	using Metadata = Aisaka::Metadata;
	using Parameter = Aisaka::Parameter;

	commands.add_command(
		Command("weather")
			.metadata(
				Metadata()
					.description(
						"Shows the current weather in the specified location.")
					.examples(
						Examples{{"Plovdiv",
								  "Searches for Plovdiv and sends information "
								  "about its current weather."}}))
			.params({Parameter("location")})
			.function(weather)
			.category(*this),
		log);
}
