#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <cpr/cpr.h>
#include <aegis.hpp>
#include <taigabot/Config.hpp>

namespace TaigaBot {
class Client {
   public:
	void load_config();
	TaigaBot::Config::Config get_config() { return this->config; };

	void set_bot(aegis::core& bot) { this->bot = &bot; }
	aegis::core* get_bot() { return this->bot; }

	void message_create(aegis::gateway::events::message_create obj);

   private:
	aegis::core* bot;
	TaigaBot::Config::Config config;
};
}  // namespace TaigaBot

#endif