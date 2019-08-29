#include <aegis.hpp>
#include <mongocxx/instance.hpp>
#include <taigabot/Commands.hpp>
#include <taigabot/Events.hpp>

int main(int argc, char* argv[]) {
	if (argc == 0) {
		printf("no token!\n");
		return 1;
	}
	aegis::core bot(spdlog::level::trace);
	bot.set_on_message_create(TaigaBot::Events::on_message);
	TaigaBot::Commands::add_commands();
	TaigaBot::Config::Config conf = TaigaBot::Config::load_config();

	mongocxx::instance instance{};
}