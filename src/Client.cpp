#include <taigabot/Command.hpp>
#include <taigabot/util/StringUtil.hpp>

void TaigaBot::Client::onMessage(SleepyDiscord::Message message) {
	auto prefix = get_config().prefix;
	if (message.startsWith(prefix)) {
		auto parameters = TaigaBot::Util::String::split_command(
			message.content.erase(0, prefix.length()), prefix);
		if (
			// only allow if has more then 1 parameter
			parameters.size() <= 1 &&
			// only allow if starts with a mention
			parameters.front() != prefix) {
			return;
		}

		// remove the parameters as we go
		parameters.pop_front();
		if (parameters.empty()) {
			return;
		}

		// get command
		TaigaBot::Command::MappedCommands::iterator foundCommand =
			TaigaBot::Command::all.find(parameters.front());
		if (foundCommand == TaigaBot::Command::all.end()) {
			sendMessage(message.channelID, "Command not found.");
			return;
		}
		parameters.pop_front();

		unsigned short requiredParams;
		for (const auto& param : foundCommand->second.params) {
			if (param.required) {
				requiredParams++;
			}
		}
		if (parameters.size() < requiredParams) {
			sendMessage(message.channelID, "Too few arguments.");
			return;
		}

		// call command
		foundCommand->second.verb(*this, message, parameters);
	}
}

TaigaBot::Config::Config TaigaBot::Client::get_config() {
	return TaigaBot::Client::config;
}

void TaigaBot::Client::set_config(TaigaBot::Config::Config config) {
	TaigaBot::Client::config = config;
}