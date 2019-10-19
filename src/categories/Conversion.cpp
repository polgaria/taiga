#include <taiga/Client.hpp>
#include <taiga/categories/Conversion.hpp>
#include <taiga/util/String.hpp>
#include <taiga/util/Various.hpp>

static void money(aegis::gateway::events::message_create& obj,
				  Taiga::Client& client,
				  const std::deque<std::string_view>& params,
				  const std::string&) {
	const auto currency_x = Taiga::Util::String::to_upper(params.front());
	const auto currency_y = Taiga::Util::String::to_upper(params[1]);

	// just.. no
	if (currency_x == currency_y) {
		obj.channel.create_message("no");

		return;
	}

	const auto _amount =
		params.size() >= 3
			? Taiga::Util::String::string_to_number<float>(params[2])
			: 1;
	if (!_amount) {
		obj.channel.create_message("Invalid parameters.");
	}
	const auto& amount = _amount.value();

	float conversion_rate;
	try {
		conversion_rate = Taiga::Util::Various::conversion_rate(
			currency_x, currency_y, client.config().currency_conv_api_key,
			client.bot().get_rest_controller());
	} catch (const std::runtime_error& error) {
		obj.channel.create_message(error.what());

		return;
	}

	auto worth = amount * conversion_rate;

	obj.channel.create_message(fmt::format(
		"{:.2f} {} is worth {:.2f} {}", amount, currency_x, worth, currency_y));
}

static void mbps(aegis::gateway::events::message_create& obj, Taiga::Client&,
				 const std::deque<std::string_view>& params,
				 const std::string&) {
	const auto _value =
		!params.empty()
			? Taiga::Util::String::string_to_number<float>(params.front())
			: 1;
	if (!_value) {
		obj.channel.create_message("Invalid parameters.");
	}
	const auto& value = _value.value();

	obj.channel.create_message(
		fmt::format("{}Mb/s is {:.2f}Mbps", value, value * 8));
}

static void mbs(aegis::gateway::events::message_create& obj, Taiga::Client&,
				const std::deque<std::string_view>& params,
				const std::string&) {
	const auto _value =
		!params.empty()
			? Taiga::Util::String::string_to_number<float>(params.front())
			: 1;
	if (!_value) {
		obj.channel.create_message("Invalid parameters.");
	}
	const auto& value = _value.value();

	obj.channel.create_message(
		fmt::format("{}Mbps is {:.2f}Mb/s", value, value / 8));
}

void Taiga::Categories::Conversion::init(
	spdlog::logger& log, Aisaka::Commands<Taiga::Client>& commands) {
	using Command = Aisaka::Command<Taiga::Client>;
	using Metadata = Aisaka::Metadata;
	using Parameter = Aisaka::Parameter;

	commands.add_command(
		Command("money")
			.category(*this)
			.metadata(
				Metadata()
					.description("Converts currency to other currency.")
					.examples(Examples{{"usd bgn", "Converts 1 USD to BGN."},
									   {"usd bgn 5", "Converts 5 USD to BGN"}}))
			.params({Parameter("currency to convert from"),
					 Parameter("currency to convert to"),
					 Parameter("amount").required(false)})
			.function(money),
		log);
	commands.add_command(  //
		Command("mbps")
			.category(*this)
			.metadata(Metadata().description("Converts Mb/s to Mbps."))
			.params({Parameter("value").required(false)})
			.function(mbps),
		log);
	commands.add_command(  //
		Command("mbs")
			.category(*this)
			.metadata(Metadata().description("Converts Mbps to Mb/s."))
			.params({Parameter("value").required(false)})
			.function(mbs),
		log);
}
