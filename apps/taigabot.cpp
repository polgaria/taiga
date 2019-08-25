#include <taigabot/Client.hpp>
#include <taigabot/Commands.hpp>

int main(int argc, char* argv[]) {
	if (argc == 0) {
		printf("no token!\n");
		return 1;
	}

	TaigaBot::Commands::add_commands();
	TaigaBot::Config::Config conf = TaigaBot::Config::load_config();

	TaigaBot::Client client(conf.token, 2);
	client.set_config(conf);
	client.run();
}