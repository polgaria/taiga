#include <taiga/Bot.hpp>
#include <taiga/util/Command.hpp>
#include <taiga/util/String.hpp>

std::optional<std::reference_wrapper<aegis::user>>
Taiga::Util::Command::find_user(const std::string_view name,
								const aegis::gateway::objects::message& msg,
								Taiga::Bot& client) {
	// in case a user is mentioned
	if (!msg.mentions.empty()) {
		const auto& member_id = msg.mentions.front();
		const auto& member = client.core().find_user(member_id);

		// don't know how this could happen but let's be safe
		if (member != nullptr) {
			return *member;
		}
	}

	// if not, (try to) find by username/nickname
	auto& m = client.core().get_user_mutex();
	std::unique_lock<aegis::shared_mutex> l(m);
	auto& users = client.core().get_user_map();

	for (auto& [_id, member] : users) {
		if (member->get_username() == name ||
			member->get_name(msg.get_guild_id()) == name) {
			l.unlock();

			return *member.get();
		}
	}
	l.unlock();

	// we assume it's an ID
	const auto member_id = Taiga::Util::String::string_to_number<int64_t>(name);
	if (member_id) {
		const auto& member = client.core().find_user(member_id.value());
		if (member != nullptr) {
			return *member;
		}
	}

	// finally, if nothing is found; nullopt
	return std::nullopt;
}
