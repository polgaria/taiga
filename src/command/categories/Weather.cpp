#include <cpr/cpr.h>
#include <aegis.hpp>
#include <taiga/command/Command.hpp>
#include <taiga/command/categories/Weather.hpp>
#include <taiga/util/String.hpp>
#include <taiga/util/Various.hpp>

Taiga::Command::Categories::Weather::Weather(const std::string& _name)
	: Taiga::Command::Category(_name) {}

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

void Taiga::Command::Categories::Weather::init(spdlog::logger& log) {
	using Metadata = Taiga::Commands::Metadata;
	using Command = Taiga::Commands::Command;

	Taiga::Commands::add_command(  //
		Command()
			.name("weather")
			.metadata(Metadata().description(""))
			.params({{"location"}})
			.function(weather)
			.category(*this),
		log);
}
