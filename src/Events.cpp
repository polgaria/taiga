#include <cpr/cpr.h>
#include <aegis.hpp>
#include <taigabot/Command.hpp>
#include <taigabot/Events.hpp>
#include <taigabot/util/StringUtil.hpp>

void TaigaBot::Events::on_message(aegis::gateway::events::message_create obj) {
	std::string prefix = "t!";
	auto content = obj.msg.get_content();
	if (content.compare(0, prefix.size(), prefix)) {
		auto parameters = TaigaBot::Util::String::split_command(
			content.erase(0, prefix.length()), prefix);
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
			obj.channel.create_message("Command not found.");
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
			obj.channel.create_message("Too few arguments.");
			return;
		}

		// call command
		foundCommand->second.verb(obj.msg, parameters);
	}
}