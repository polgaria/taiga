#pragma once

#include <aisaka/Client.hpp>
#include <aisaka/command/Commands.hpp>
#include <taiga/Config.hpp>

namespace Taiga {
class Client : public Aisaka::Client {
   public:
	Client() : Aisaka::Client("", "", 0) {}
	Client(const std::string& _default_prefix, const std::string& _bot_name,
		   const int64_t& _owner_id)
		: Aisaka::Client(_default_prefix, _bot_name, _owner_id) {}
	virtual ~Client() override = default;

	void load_config();
	[[nodiscard]] Taiga::Config::Config& config() noexcept {
		return this->_config;
	}

	[[nodiscard]] const Aisaka::Commands<Taiga::Client>& commands() const
		noexcept {
		return this->_commands;
	}

	[[nodiscard]] std::unordered_multimap<int64_t, std::string>&
	prefix_cache() noexcept {
		return this->_prefix_cache;
	}

	void load_values_from_config();

	void load_categories(spdlog::logger& log);

	virtual void on_message_create(
		aegis::gateway::events::message_create obj) override;

   private:
	Taiga::Config::Config _config;
	Aisaka::Commands<Taiga::Client> _commands;

	std::unordered_multimap<int64_t, std::string> _prefix_cache;
};
}  // namespace Taiga
