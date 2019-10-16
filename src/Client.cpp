#include <aisaka/util/String.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/client.hpp>
#include <taiga/Client.hpp>
#include <taiga/categories/Conversion.hpp>
#include <taiga/categories/General.hpp>
#include <taiga/categories/Miscellaneous.hpp>
#include <taiga/categories/Prefix.hpp>
#include <taiga/categories/Reddit.hpp>
#include <taiga/categories/Timezone.hpp>
#include <taiga/categories/Weather.hpp>
#include <taiga/util/String.hpp>

// TODO: just.. why
#define ENTRY(name, error_message)                         \
	if (config_json.find(#name) != config_json.end() &&    \
		config_json[#name].is_string()) {                  \
		conf.name = config_json[#name].get<std::string>(); \
	} else {                                               \
		throw std::runtime_error(error_message);           \
	}

#define OPTIONAL_ENTRY(name)                               \
	if (config_json.find(#name) != config_json.end() &&    \
		config_json[#name].is_string()) {                  \
		conf.name = config_json[#name].get<std::string>(); \
	}

#define ENTRY_STR_TO_HEX_OR(name, _or)                         \
	if (config_json.find(#name) != config_json.end() &&        \
		config_json[#name].is_string()) {                      \
		conf.name = Taiga::Util::String::string_to_hex<int>(   \
						config_json[#name].get<std::string>()) \
						.value_or(_or);                        \
	} else {                                                   \
		conf.name = _or;                                       \
	}

#define INIT_CATEGORY(category) \
	Taiga::Categories::category{#category}.init(log, this->_commands)

void Taiga::Client::message_create(aegis::gateway::events::message_create obj) {
	using bsoncxx::builder::stream::document;
	using bsoncxx::builder::stream::finalize;

	// don't want it to be responding to other bots
	if (obj.msg.get_user().is_bot()) {
		return;
	}

	std::string_view content = obj.msg.get_content();
	const auto& guild_id = obj.channel.get_guild_id();
	std::string_view prefix;

	// try to find an existing custom prefix
	// if not found in cache, try to get from DB
	// if not found in DB, use default prefix
	if (this->prefix_cache.count(guild_id) > 0) {
		const auto& range = this->prefix_cache.equal_range(guild_id);
		const auto& prefix_in_cache = std::find_if(
			range.first, range.second, [&content](const auto& _prefix) {
				return !content.compare(0, _prefix.second.length(),
										_prefix.second);
			});
		if (prefix_in_cache != range.second) {
			prefix = prefix_in_cache->second;
		}
	}

	if (prefix.empty()) {
		const auto& mongo_client = get_mongo_pool().acquire();
		const auto& op_result =
			(*mongo_client)[this->bot_name]["prefixes"].find_one(
				document{} << "id" << obj.msg.get_guild().get_id() << finalize);
		if (!op_result && !content.compare(0, this->default_prefix.size(),
										   this->default_prefix)) {
			prefix = this->default_prefix;
			this->prefix_cache.emplace(guild_id, prefix);
		} else if (op_result) {
			for (const auto& res :
				 op_result->view()["prefix"].get_array().value) {
				const auto& _prefix = res.get_utf8().value.to_string();
				this->prefix_cache.emplace(guild_id, _prefix);
				if (!content.compare(0, _prefix.length(), _prefix)) {
					prefix = std::move(_prefix);
					break;
				}
			}
		}
	}

	// check if it starts with the configured prefix
	if (!prefix.empty()) {
		content.remove_prefix(prefix.length());
		auto params = Aisaka::Util::String::split_command(content, prefix);
		if (
			// only allow if it has at least 1 parameter
			params.size() <= 1 &&
			// only allow if starts with prefix
			params.front() != prefix) {
			return;
		}

		// remove the params as we go
		params.pop_front();
		if (params.empty()) {
			return;
		}

		// get command
		const auto& _found_command = this->get_commands().find_command(
			Aisaka::Util::String::to_lower(params.front()));
		if (!_found_command) {
			obj.channel.create_message("Command not found.");
			return;
		}
		const auto& found_command = _found_command.value();
		params.pop_front();

		// if the command is owner-only
		if (found_command.owner_only()) {
			// check if user is the bot owner
			const auto& user_id = obj.msg.get_user().get_id().get();
			if (user_id != this->owner_id) {
				obj.channel.create_message("You are not the bot's owner!");
				return;
			}
		}

		// check how many parameters are required
		unsigned short required_params = 0;
		for (const auto& param : found_command.params()) {
			if (param.required()) {
				required_params++;
			}
		}
		if (params.size() < required_params) {
			obj.channel.create_message("Too few parameters.");
			return;
		}

		// call command
		found_command.function()(obj, *this, params, prefix.data());
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
	OPTIONAL_ENTRY(weather_api_key)
	OPTIONAL_ENTRY(git_repo)
	OPTIONAL_ENTRY(owner_id)
	ENTRY_STR_TO_HEX_OR(color, 0xEDC5B9)

	this->config = conf;
}

void Taiga::Client::load_values_from_config() {
	this->bot_name = this->config.name;
	this->default_prefix = this->config.prefix;
	if (this->config.owner_id) {
		this->owner_id = Taiga::Util::String::string_to_number<int64_t>(
							 this->config.owner_id.value())
							 .value_or(0);
	}
}

void Taiga::Client::load_categories(spdlog::logger& log) {
	INIT_CATEGORY(General);
	INIT_CATEGORY(Prefix);
	INIT_CATEGORY(Reddit);
	INIT_CATEGORY(Conversion);
	INIT_CATEGORY(Timezone);
	INIT_CATEGORY(Miscellaneous);
	INIT_CATEGORY(Weather);
}

const Aisaka::Commands<Taiga::Client>& Taiga::Client::get_commands() const
	noexcept {
	return this->_commands;
}
