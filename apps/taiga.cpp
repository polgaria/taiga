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
	Taiga::Bot bot;

	bot.load_config();
	bot.load_values_from_config();

	bot.load_categories();

	bot.core().run();
	bot.core().yield();
}