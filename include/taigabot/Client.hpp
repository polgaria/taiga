#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <cpr/cpr.h>
#include <sleepy_discord/websocketpp_websocket.h>

namespace TaigaBot {
class Client : public SleepyDiscord::DiscordClient {
   private:
	using SleepyDiscord::DiscordClient::DiscordClient;
	void onMessage(SleepyDiscord::Message message) override;
};
}  // namespace TaigaBot

#endif
