#pragma once

#include <aegis/gateway/objects/message.hpp>
#include <deque>
#include <fifo_map.hpp>
#include <functional>
#include <iostream>
#include <taiga/Client.hpp>
#include <taiga/Config.hpp>
#include <taiga/command/categories/Category.hpp>
#include <unordered_map>
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

	class Metadata {
	   public:
		const std::string& description() const noexcept;
		Metadata& description(const std::string& description) noexcept;

		const std::unordered_map<std::string, std::string>& examples() const
			noexcept;
		Metadata& examples(const std::unordered_map<std::string, std::string>&
							   examples) noexcept;

	   private:
		std::string _description;
		std::unordered_map<std::string, std::string> _examples;
	};

	class Command {
	   public:
		const std::string& name() const noexcept;
		Command& name(const std::string& name) noexcept;

		const Taiga::Command::Category& category() const noexcept;
		Command& category(const Taiga::Command::Category& category) noexcept;

		const std::deque<Parameter>& params() const noexcept;
		Command& params(const std::deque<Parameter>& params) noexcept;

		const std::unordered_set<std::string>& aliases() const noexcept;
		Command& aliases(
			const std::unordered_set<std::string>& aliases) noexcept;

		const Metadata& metadata() const noexcept;
		Command& metadata(const Metadata& metadata) noexcept;

		const Function& function() const noexcept;
		Command& function(const Function& function) noexcept;

		const bool& owner_only() const noexcept;
		Command& owner_only(const bool& is_owner_only) noexcept;

	   private:
		std::string _name;
		Taiga::Command::Category _category = {"None"};
		std::deque<Parameter> _params;
		std::unordered_set<std::string> _aliases;
		Metadata _metadata = Metadata();
		Function _function;
		bool _owner_only = false;
	};

	using MappedCommands = nlohmann::fifo_map<std::string, Command>;
	static MappedCommands all;
	static std::unordered_map<std::string, Taiga::Command::Category> categories;

	static void add_command(Command command, spdlog::logger& log);
	static void add_commands(spdlog::logger& log);
};
}  // namespace Taiga
