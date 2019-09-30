#pragma once

#include <deque>
#include <fifo_map.hpp>
#include <functional>
#include <taiga/Client.hpp>
#include <taiga/Config.hpp>
#include <taiga/command/categories/Category.hpp>
#include <unordered_set>

namespace Taiga {
class Commands {
   public:
	using Function = std::function<void(
		aegis::gateway::events::message_create& obj,
		const std::deque<std::string>& params,
		const std::string& command_prefix, Taiga::Client& client)>;

	class Parameter {
	   public:
		Parameter(std::string_view name);

		const std::string_view& name() const noexcept;
		Parameter& name(const std::string_view& name) noexcept;

		const bool& required() const noexcept;
		Parameter& required(const bool& required) noexcept;

	   private:
		std::string_view _name;
		bool _required = true;
	};

	class Metadata {
	   public:
		const std::string& description() const noexcept;
		Metadata& description(const std::string& description) noexcept;

		const nlohmann::fifo_map<std::string, std::string>& examples() const
			noexcept;
		Metadata& examples(const nlohmann::fifo_map<std::string, std::string>&&
							   examples) noexcept;

	   private:
		std::string _description;
		nlohmann::fifo_map<std::string, std::string> _examples;
	};

	class Command {
	   public:
		Command();
		Command(std::string name);

		const std::string& name() const noexcept;
		Command& name(const std::string& name) noexcept;

		const Taiga::Category& category() const noexcept;
		Command& category(const Taiga::Category& category) noexcept;

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
		Taiga::Category _category = Taiga::Category("None");
		std::deque<Parameter> _params;
		std::unordered_set<std::string> _aliases;
		Metadata _metadata = Metadata();
		Function _function;
		bool _owner_only = false;
	};

	using MappedCommands = nlohmann::fifo_map<std::string, Command>;
	static MappedCommands all;
	static std::unordered_map<std::string, Taiga::Category> categories;

	static void add_command(Command command, spdlog::logger& log);
	static void add_commands(spdlog::logger& log);
};
}  // namespace Taiga
