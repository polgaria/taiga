#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <memory>
#include <mongocxx/client.hpp>
#include <taiga/Client.hpp>
#include <taiga/command/Command.hpp>
#include <taiga/util/String.hpp>

#define ENTRY(name, error_message)                         \
	if (config_json.find(#name) != config_json.end() &&    \
		config_json[#name].is_string()) {                  \
		conf.name = config_json[#name].get<std::string>(); \
                                                           \
	} else {                                               \
		throw std::runtime_error(error_message);           \
	}

#define OPTIONAL_ENTRY(name)                               \
	if (config_json.find(#name) != config_json.end() &&    \
		config_json[#name].is_string()) {                  \
		conf.name = config_json[#name].get<std::string>(); \
	}

void Taiga::Client::message_create(aegis::gateway::events::message_create obj) {
	using bsoncxx::builder::stream::document;
	using bsoncxx::builder::stream::finalize;

	// don't want it to be responding to other bots
	if (obj.msg.get_user().is_bot()) {
		return;
	}
	auto content{obj.msg.get_content()};

	// try to find an existing custom prefix
	// if not found, use default prefix
	std::string prefix{};
	const auto& mongo_client = get_mongo_pool().acquire();
	const auto& op_result = (*mongo_client)["taiga"]["prefixes"].find_one(
		document{} << "id" << obj.msg.get_guild().get_id() << finalize);
	if (!op_result &&
		!content.compare(0, config.prefix.size(), config.prefix)) {
		prefix = config.prefix;
	} else if (op_result) {
		for (const auto& res : op_result->view()["prefix"].get_array().value) {
			const auto& _prefix = std::string{res.get_utf8().value};
			if (!content.compare(0, _prefix.size(), _prefix)) {
				prefix = _prefix;
				break;
			}
		}
	} else {
		return;
	}

	// check if it starts with the configured prefix
	if (!prefix.empty()) {
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
		const auto& command_name =
			Taiga::Util::String::to_lower(params.front());
		if (params.empty()) {
			return;
		}

		// get command
		const auto& found_command = Taiga::Commands::all.find(command_name);
		if (found_command == Taiga::Commands::all.end()) {
			obj.channel.create_message("Command not found.");
			return;
		}
		params.pop_front();

		// if the command is owner-only
		if (found_command->second.owner_only()) {
			// check if user is the bot owner
			const auto& user_id = obj.msg.get_user().get_id().get();
			if (fmt::format("{}", user_id) != config.owner_id.value_or("")) {
				obj.channel.create_message("You are not the bot's owner!");
				return;
			}
		}

		// check how many parameters are required
		unsigned short required_params = 0;
		for (const auto& param : found_command->second.params()) {
			if (param.required) {
				required_params++;
			}
		}
		if (params.size() < required_params) {
			obj.channel.create_message("Too few parameters.");
			return;
		}

		// call command
		found_command->second.function()(obj, params, prefix, *this);
	}
}

void Taiga::Client::load_config() {
	std::ifstream config_file("config.json");

	if (config_file.fail()) {
		throw std::runtime_error("No config found.");
	}

	nlohmann::json config_json;
	config_file >> config_json;

	config_file.close();

	Taiga::Config::Config conf;
	ENTRY(prefix, "Invalid config: The bot prefix is missing.")
	ENTRY(name, "Invalid config: The bot name is missing.")
	OPTIONAL_ENTRY(currency_conv_api_key)
	OPTIONAL_ENTRY(git_repo)
	OPTIONAL_ENTRY(owner_id)

	this->config = conf;
}
