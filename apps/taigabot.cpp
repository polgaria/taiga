#include <aegis.hpp>
#include <mongocxx/instance.hpp>
#include <taigabot/Client.hpp>
#include <taigabot/Commands.hpp>
#include <taigabot/Config.hpp>

int main(int argc, char* argv[]) {
	aegis::core bot(spdlog::level::trace);

	TaigaBot::Client client{};
	client.set_bot(bot);
	client.load_config();

	bot.set_on_message_create(std::bind(&TaigaBot::Client::message_create,
										&client, std::placeholders::_1));

	TaigaBot::Commands::add_commands(bot.log);
	mongocxx::instance instance{};

	bot.run();
	bot.yield();
}