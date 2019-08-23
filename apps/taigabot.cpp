#include <taigabot/Client.hpp>

int main(int argc, char* argv[]) {
	if (argc == 0) {
		printf("no token!\n");
		return 1;
	}

	TaigaBot::Client client(argv[1], 2);
	client.run();
}
