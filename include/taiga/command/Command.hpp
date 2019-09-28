#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <aegis/gateway/objects/message.hpp>
#include <deque>
#include <fifo_map.hpp>
#include <functional>
#include <iostream>
#include <taiga/Client.hpp>
#include <taiga/Config.hpp>
#include <unordered_set>

namespace Taiga {
class Commands {
   public:
	using Function = std::function<void(
		aegis::gateway::events::message_create& obj,
		const std::deque<std::string>& params,
		const std::string& command_prefix, Taiga::Client& client)>;

	struct Parameter {
		std::string_view name;
		bool required = true;
	};

	class Command {
	   public:
		std::string& name() noexcept;
		Command& name(const std::string& name) noexcept;

		std::string& category() noexcept;
		Command& category(const std::string& category) noexcept;

		std::deque<Parameter>& params() noexcept;
		Command& params(const std::deque<Parameter>& params) noexcept;

		std::unordered_set<std::string>& aliases() noexcept;
		Command& aliases(
			const std::unordered_set<std::string>& aliases) noexcept;

		std::string& description() noexcept;
		Command& description(const std::string& description) noexcept;

		Function& function() noexcept;
		Command& function(const Function& function) noexcept;

		bool& owner_only() noexcept;
		Command& owner_only(const bool& is_owner_only) noexcept;

	   private:
		std::string _name;
		std::string _category;
		std::deque<Parameter> _params = {};
		std::unordered_set<std::string> _aliases;
		std::string _description;
		Function _function;
		bool _owner_only = false;
	};

	using MappedCommands = nlohmann::fifo_map<std::string, Command>;
	static MappedCommands all;

	static void add_command(Command command, spdlog::logger& log);
	static void add_commands(spdlog::logger& log);
};
}  // namespace Taiga

#endif
