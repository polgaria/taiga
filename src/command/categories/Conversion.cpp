#include <aegis.hpp>
#include <taiga/command/Command.hpp>
#include <taiga/command/categories/Conversion.hpp>
#include <taiga/util/String.hpp>
#include <taiga/util/Various.hpp>

Taiga::Command::Categories::Conversion::Conversion(const std::string& _name)
	: Taiga::Command::Category(_name) {}

COMMAND(money) {
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
			currency_x, currency_y, client.get_config().currency_conv_api_key,
			client.get_bot().get_rest_controller());
	} catch (const std::runtime_error& error) {
		obj.channel.create_message(error.what());

		return;
	}

	auto worth = amount * conversion_rate;

	obj.channel.create_message(fmt::format(
		"{:.2f} {} is worth {:.2f} {}", amount, currency_x, worth, currency_y));
}

COMMAND(mbps) {
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

COMMAND(mbs) {
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

void Taiga::Command::Categories::Conversion::init(spdlog::logger& log) {
	using Metadata = Taiga::Commands::Metadata;
	using Command = Taiga::Commands::Command;

	Taiga::Commands::add_command(
		Taiga::Commands::Command()
			.name("money")
			.category(*this)
			.metadata(Metadata()
						  .description("Converts currency to other currency.")
						  .examples({{"usd bgn", "Converts 1 USD to BGN."},
									 {"usd bgn 5", "Converts 5 USD to BGN"}}))
			.params({{"currency to convert from"},
					 {"currency to convert to"},
					 {"amount", false}})
			.function(money),
		log);
	Taiga::Commands::add_command(  //
		Taiga::Commands::Command()
			.name("mbps")
			.category(*this)
			.metadata(Metadata().description("Converts Mb/s to Mbps."))
			.params({{"value", false}})
			.function(mbps),
		log);
	Taiga::Commands::add_command(  //
		Taiga::Commands::Command()
			.name("mbs")
			.category(*this)
			.metadata(Metadata().description("Converts Mbps to Mb/s."))
			.params({{"value", false}})
			.function(mbs),
		log);
}
