#define ASIO_NO_DEPRECATED
#define HAS_UNCAUGHT_EXCEPTIONS 1

#ifdef _WIN32
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS
#define _HAS_AUTO_PTR_ETC = 1
#endif

#include <aegis.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/pool.hpp>
#include <taiga/Bot.hpp>
#include <taiga/Config.hpp>

int main() {
	aegis::core bot(spdlog::level::trace);

	mongocxx::instance instance{};

	Taiga::Bot client{};
	client.core(bot);

	auto mongo_pool = mongocxx::pool{mongocxx::uri{}};
	client.mongo_pool(std::move(mongo_pool));

	client.load_config();
	client.load_values_from_config();

	client.load_categories(*bot.log);
	bot.set_on_message_create(std::bind(&Taiga::Bot::on_message_create, &client,
										std::placeholders::_1));

	bot.run();
	bot.yield();
}