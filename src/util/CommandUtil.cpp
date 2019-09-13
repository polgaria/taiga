#include <taiga/Client.hpp>
#include <taiga/util/CommandUtil.hpp>

std::unique_ptr<aegis::user>& Taiga::Util::Command::find_user(
	const std::string& name,
	const aegis::snowflake& guild_id,
	Taiga::Client* client) {
	auto& m = client->get_bot()->get_user_mutex();
	std::unique_lock<aegis::shared_mutex> l(m);
	auto& users = client->get_bot()->get_user_map();

	for (auto& [_id, member] : users) {
		auto nickname{member->get_name(guild_id)};
		auto matches_nickname = nickname == name;

		if (member->get_username() == name || nickname == name) {
			l.unlock();
			return member;
		}
	}

	l.unlock();
	throw std::runtime_error("Could not find member!");
}
