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
	Taiga::Config::Config& get_config() noexcept { return this->config; }

	void load_values_from_config();

	const Aisaka::Commands<Taiga::Client>& get_commands() const noexcept;

	void load_categories(spdlog::logger& log);

	virtual void message_create(
		aegis::gateway::events::message_create obj) override;

   private:
	Taiga::Config::Config config;
	Aisaka::Commands<Taiga::Client> _commands;
};
}  // namespace Taiga
