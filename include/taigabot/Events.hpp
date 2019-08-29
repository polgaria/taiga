#ifndef EVENTS_HPP
#define EVENTS_HPP

#include <aegis.hpp>
#include <mongocxx/client.hpp>
#include <taigabot/Config.hpp>

namespace TaigaBot::Events {
void on_message(aegis::gateway::events::message_create obj);
}  // namespace TaigaBot::Events

#endif