#include <taiga/command/Command.hpp>
#include <taiga/command/categories/Reddit.hpp>
#include <taiga/util/Various.hpp>

Taiga::Command::Categories::Reddit::Reddit(const std::string& _name)
	: Taiga::Command::Category(_name) {}

COMMAND(taiga) {
	obj.channel.create_message(
		Taiga::Util::Various::get_random_reddit_post_url("taiga"));
}

COMMAND(toradora) {
	obj.channel.create_message(
		Taiga::Util::Various::get_random_reddit_post_url("toradora"));
}

void Taiga::Command::Categories::Reddit::init(spdlog::logger& log) {
	Taiga::Commands::add_command(
		Taiga::Commands::Command()
			.name("taiga")
			.description("Sends a random image from r/taiga")
			.function(taiga)
			.category(*this),
		log);
	Taiga::Commands::add_command(
		Taiga::Commands::Command()
			.name("toradora")
			.description("Sends a random image from r/toradora")
			.function(toradora)
			.category(*this),
		log);
}
