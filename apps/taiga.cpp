#define ASIO_NO_DEPRECATED
#define HAS_UNCAUGHT_EXCEPTIONS 1

#include <aegis/core.hpp>
#include <aegis/gateway/events/message_create.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/pool.hpp>
#include <taiga/Client.hpp>
#include <taiga/Config.hpp>
#include <taiga/command/Commands.hpp>

int main() {
	aegis::core bot(spdlog::level::trace);

	mongocxx::instance instance{};

	Taiga::Client client{};
	client.set_bot(bot);

	auto mongo_pool = mongocxx::pool{mongocxx::uri{}};
	client.set_mongo_pool(std::move(mongo_pool));

	client.load_config();

	bot.set_on_message_create(std::bind(&Taiga::Client::message_create, &client,
										std::placeholders::_1));
	Taiga::Commands::add_commands(*bot.log);

	bot.run();
	bot.yield();
}