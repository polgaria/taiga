#include <memory>
#include <taiga/Client.hpp>
#include <taiga/Command.hpp>
#include <taiga/util/StringUtil.hpp>

void Taiga::Client::message_create(aegis::gateway::events::message_create obj) {
	auto content{obj.msg.get_content()};
	const auto& prefix = get_config().prefix;
	if (!content.compare(0, prefix.size(), prefix)) {
		auto params = Taiga::Util::String::split_command(
			content.erase(0, prefix.length()), prefix);
		if (
			// only allow if it has at least 1 parameter
			params.size() <= 1 &&
			// only allow if starts with prefix
			params.front() != prefix) {
			return;
		}

		// remove the params as we go
		params.pop_front();
		// make command name lower-case
		params.front() = Taiga::Util::String::to_lower(params.front());
		if (params.empty()) {
			return;
		}

		// get command
		const auto& foundCommand = Taiga::Command::all.find(params.front());
		if (foundCommand == Taiga::Command::all.end()) {
			obj.channel.create_message("Command not found.");
			return;
		}
		params.pop_front();

		unsigned short requiredParams;
		for (const auto& param : foundCommand->second.params) {
			if (param.required) {
				requiredParams++;
			}
		}
		if (params.size() < requiredParams) {
			obj.channel.create_message("Too few arguments.");
			return;
		}

		// call command
		foundCommand->second.verb(obj, params, *this);
	}
}

void Taiga::Client::load_config() {
#define ENTRY(name, error_message)                                   \
	if (json.find(#name) != json.end() && json[#name].is_string()) { \
		conf.name = json[#name].get<std::string>();                  \
                                                                     \
	} else {                                                         \
		throw std::runtime_error(error_message);                     \
	}

#define OPTIONAL_ENTRY(name)                                              \
	conf.name = json.find(#name) != json.end() && json[#name].is_string() \
					? json[#name].get<std::string>()                      \
					: "";

	std::ifstream i("config.json");

	if (i.fail()) {
		throw std::runtime_error("No config found.");
	}

	nlohmann::json json;
	i >> json;

	i.close();

	Taiga::Config::Config conf;
	ENTRY(prefix, "Invalid config: The bot prefix is missing.");
	OPTIONAL_ENTRY(currency_conv_api_key);

	this->config = conf;
}