#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <cpr/cpr.h>
#include <aegis.hpp>
#include <taiga/Config.hpp>

namespace Taiga {
class Client {
   public:
	void load_config();
	Taiga::Config::Config get_config() { return this->config; };

	void set_bot(aegis::core& bot) { this->bot = &bot; }
	aegis::core* get_bot() { return this->bot; }

	void message_create(aegis::gateway::events::message_create obj);

   private:
	aegis::core* bot;
	Taiga::Config::Config config;
};
}  // namespace Taiga

#endif