#pragma once

#include <aisaka/Bot.hpp>
#include <aisaka/command/Commands.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/pool.hpp>
#include <taiga/Config.hpp>

namespace Taiga {
class Bot : public Aisaka::Bot {
   public:
	Bot() : Aisaka::Bot("", "", 0) {
		mongocxx::instance instance{};

		this->load_config();
		this->load_values_from_config();
	}
	virtual ~Bot() override = default;

	void load_config();
	[[nodiscard]] inline Taiga::Config::Config& config() noexcept {
		return this->_config;
	}
	[[nodiscard]] const inline Aisaka::Commands<Taiga::Bot>& commands() const
		noexcept {
		return this->_commands;
	}
	[[nodiscard]] inline mongocxx::pool& mongo_pool() noexcept {
		return this->_mongo_pool;
	}

	[[nodiscard]] inline std::unordered_multimap<int64_t, std::string>&
	prefix_cache() noexcept {
		return this->_prefix_cache;
	}

	void load_values_from_config();
	void load_categories();

	virtual void on_message_create(
		aegis::gateway::events::message_create obj) override;

   private:
	Taiga::Config::Config _config;
	Aisaka::Commands<Taiga::Bot> _commands;
	mongocxx::pool _mongo_pool;

	std::unordered_multimap<int64_t, std::string> _prefix_cache;
};
}  // namespace Taiga
