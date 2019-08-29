#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <cpr/cpr.h>
#include <sleepy_discord/websocketpp_websocket.h>
#include <mongocxx/client.hpp>
#include <taigabot/Config.hpp>

namespace TaigaBot {
class Client : public SleepyDiscord::DiscordClient {
   public:
	TaigaBot::Config::Config get_config();
	void set_config(TaigaBot::Config::Config conf);

   private:
	TaigaBot::Config::Config config;
	using SleepyDiscord::DiscordClient::DiscordClient;
	void onMessage(SleepyDiscord::Message message) override;
};
}  // namespace TaigaBot

#endif