#pragma once

#include <aegis.hpp>
#include <mongocxx/pool.hpp>
#include <taiga/Config.hpp>

namespace Taiga {
class Client {
   public:
	void load_config();
	Taiga::Config::Config get_config() { return this->config; }

	void set_bot(aegis::core& _bot) { this->bot = &_bot; }
	aegis::core& get_bot() { return *bot; }

	void set_mongo_pool(mongocxx::pool&& client) { this->mongo_pool = &client; }
	mongocxx::pool& get_mongo_pool() { return *mongo_pool; }

	void message_create(aegis::gateway::events::message_create obj);

   private:
	aegis::core* bot;
	mongocxx::pool* mongo_pool;
	Taiga::Config::Config config;
};
}  // namespace Taiga
