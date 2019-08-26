#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <taigabot/Config.hpp>

#define ENTRY(name, error_message)                       \
	if (doc.HasMember(#name) && doc[#name].IsString()) { \
		conf.name = doc[#name].GetString();              \
	} else {                                             \
		throw std::runtime_error(error_message);         \
	}

#define OPTIONAL_ENTRY(name)                                  \
	conf.name = doc.HasMember(#name) && doc[#name].IsString() \
					? doc[#name].GetString()                  \
					: "";

TaigaBot::Config::Config TaigaBot::Config::load_config() {
	FILE* file = fopen("config.json", "rb");
	rapidjson::Document doc;

	if (!file) {
		throw std::runtime_error("No config found.");
	};

	char readBuffer[65536];
	rapidjson::FileReadStream readStream(file, readBuffer, sizeof(readBuffer));

	doc.ParseStream(readStream);
	fclose(file);

	Config conf;
	ENTRY(prefix, "Invalid config: The bot prefix is missing.");
	ENTRY(token, "Invalid config: The bot token is missing");
	OPTIONAL_ENTRY(currency_api_token);

	return conf;
}