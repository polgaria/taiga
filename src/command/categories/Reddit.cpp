#include <taiga/command/Command.hpp>
#include <taiga/command/categories/Reddit.hpp>
#include <taiga/util/Util.hpp>

Taiga::Command::Categories::Reddit::Reddit(const std::string& _name)
	: Taiga::Command::Category(_name) {}

COMMAND(taiga) {
	obj.channel.create_message(
		Taiga::Util::get_random_reddit_post_url("taiga"));
}

COMMAND(toradora) {
	obj.channel.create_message(
		Taiga::Util::get_random_reddit_post_url("toradora"));
}

void Taiga::Command::Categories::Reddit::init(spdlog::logger& log) {
	ADD_COMMAND_DESC(taiga, "Sends a random image from r/taiga", {});
	ADD_COMMAND_DESC(toradora, "Sends a random image from r/toradora", {});
}
