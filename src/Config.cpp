#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <taigabot/Config.hpp>

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
	if (doc.HasMember("currency_api_token") && doc["token"].IsString()) {
		conf.token = doc["token"].GetString();
	} else {
		throw std::runtime_error("Invalid config: The bot token is missing.");
	}
	conf.currency_api_token = doc.HasMember("currency_api_token") &&
									  doc["currency_api_token"].IsString()
								  ? doc["currency_api_token"].GetString()
								  : "";

	return conf;
}