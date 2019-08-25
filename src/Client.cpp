#include <taigabot/Command.hpp>
#include <taigabot/util/StringUtil.hpp>

#define MATCH(command, s) command.startsWith(s)

void TaigaBot::Client::onMessage(SleepyDiscord::Message message) {
	auto id = getCurrentUser().cast().ID;
	if (message.isMentioned(id)) {
		auto parameters =
			TaigaBot::Util::String::split_command(message.content);
		const auto mention = "<@" + id.string() + ">";
		const auto mentionNick = "<@!" + id.string() + ">";
		if (
			// only allow if has more then 1 parameter
			parameters.size() <= 1 &&
			// only allow if starts with a mention
			(parameters.front() != mention ||
			 parameters.front() != mentionNick)) {
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
		if (parameters.size() < foundCommand->second.params.size()) {
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

void TaigaBot::Client::set_config(TaigaBot::Config::Config conf) {
	TaigaBot::Client::config = conf;
}