#include <cpr/cpr.h>
#include <aegis/channel.hpp>
#include <aegis/gateway/events/message_create.hpp>
#include <taiga/command/Commands.hpp>
#include <taiga/command/categories/Weather.hpp>
#include <taiga/util/String.hpp>
#include <taiga/util/Various.hpp>

Taiga::Categories::Weather::Weather(const std::string& _name)
	: Taiga::Category(_name) {}

COMMAND(weather) {
	if (!client.get_config().weather_api_key) {
		obj.channel.create_message("API key not set!");
		return;
	}

	const auto& location = Taiga::Util::String::join(params, "%20");
	try {
		obj.channel.create_message_embed(aegis::create_message_t().embed(
			Taiga::Util::Various::get_weather_embed(
				client.get_config().weather_api_key.value(), location)));
	} catch (const std::runtime_error& error) {
		obj.channel.create_message(error.what());
		return;
	}
}

void Taiga::Categories::Weather::init(spdlog::logger& log) {
	using Metadata = Taiga::Commands::Metadata;
	using Command = Taiga::Commands::Command;
	using Parameter = Taiga::Commands::Parameter;

	Taiga::Commands::add_command(
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
