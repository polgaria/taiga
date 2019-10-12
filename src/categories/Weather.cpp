#include <cpr/cpr.h>

#include <aegis/channel.hpp>
#include <aegis/gateway/events/message_create.hpp>
#include <aisaka/util/String.hpp>
#include <taiga/Client.hpp>
#include <taiga/categories/Weather.hpp>
#include <taiga/util/Various.hpp>

static void weather(aegis::gateway::events::message_create& obj,
					Taiga::Client& client,
					const std::deque<std::string_view>& params,
					const std::string&) {
	if (!client.get_config().weather_api_key) {
		obj.channel.create_message("API key not set!");
		return;
	}

	const auto& location = Aisaka::Util::String::join(params, "%20");
	try {
		obj.channel.create_message_embed(aegis::create_message_t().embed(
			Taiga::Util::Various::get_weather_embed(
				client.get_config().weather_api_key.value(), location)));
	} catch (const std::runtime_error& error) {
		obj.channel.create_message(error.what());
		return;
	}
}

void Taiga::Categories::Weather::init(
	spdlog::logger& log, Aisaka::Commands<Taiga::Client>& commands) {
	using Command = Aisaka::Command<Taiga::Client>;
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
