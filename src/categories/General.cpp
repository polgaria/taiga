#include <date/date.h>

#include <aisaka/Version.hpp>
#include <aisaka/util/String.hpp>
#include <taiga/Version.hpp>
#include <taiga/categories/General.hpp>
#include <taiga/util/Command.hpp>
#include <taiga/util/Math.hpp>
#include <taiga/util/String.hpp>
#include <taiga/util/Various.hpp>

static void help(aegis::gateway::events::message_create& obj,
				 Taiga::Bot& client, const std::deque<std::string>& params,
				 const std::string_view command_prefix) {
	using aegis::gateway::objects::field;

	const auto& commands = client.commands().all;

	// if any parameters were passed
	if (!params.empty()) {
		std::string_view type;
		if (params.size() > 1) {
			type = Aisaka::Util::String::to_lower(params[1]);
		}

		if (!type.empty() && type != "command" && type != "category") {
			obj.channel.create_message("Invalid explicit type!");
			return;
		}

		const auto& name = Aisaka::Util::String::to_lower(params.front());
		const auto& categories = client.commands().categories;

		// try to find a command
		// jesus christ this is ugly
		const auto& found_command = std::find_if(
			commands.begin(), commands.end(), [&name](const auto& command) {
				return Aisaka::Util::String::to_lower(command.first) == name;
			});
		// if no command was found (or the type explicitly stated is category)
		if (found_command == commands.end() || type == "category") {
			const auto& _found_category =
				// jesus christ this is ugly
				std::find_if(categories.begin(), categories.end(),
							 [&name](const auto& category) {
								 return Aisaka::Util::String::to_lower(
											category.second.name()) == name;
							 });

			// then try to find a category
			if (_found_category != categories.end() && type != "command") {
				const auto& found_category = _found_category->second;

				auto embed =
					aegis::gateway::objects::embed()
						.title(fmt::format("**{}**", found_category.name()))
						.color(client.config().color);
				std::string output;

				// find commands in that category
				for (auto& command : commands) {
					if (command.second.category().name() ==
						found_category.name()) {
						output += fmt::format(
							!command.second.metadata().description().empty()
								? "**{}** - {}\n"
								: "**{}**\n",
							command.second.name(),
							command.second.metadata().description());
					}
				}

				embed.description(std::move(output));
				obj.channel.create_message_embed(
					aegis::create_message_t().embed(embed));
				return;
			}

			if (type == "command") {
				obj.channel.create_message("No command found.");
			} else if (type == "category") {
				obj.channel.create_message("No category found.");
			} else {
				obj.channel.create_message("No command or category found.");
			}

			return;
		}

		std::vector<field> fields;
		fields.reserve(1);

		const auto& command = commands.at(name.data());

		auto embed = aegis::gateway::objects::embed()
						 .title(fmt::format("**{}**", name))
						 .color(client.config().color);

		auto syntax = fmt::format(command.params().empty() ? "`{}{}" : "`{}{} ",
								  command_prefix, name);
		for (const auto& param : command.params()) {
			syntax +=
				fmt::format(param.required() ? "<{}> " : "[{}] ", param.name());
		}
		syntax += '`';

		fields.push_back({field()
							  .name("**Syntax**")
							  .value(std::move(syntax))
							  .is_inline(true)});

		if (!command.metadata().description().empty()) {
			embed.description(command.metadata().description());
		}
		if (!command.aliases().empty()) {
			const auto& aliases = command.aliases();

			std::string aliases_string;

			// if the command name used is an alias, add the actual command name
			if (aliases.find(name) != aliases.end()) {
				aliases_string += fmt::format("`{}` ", command.name());
			}
			for (const auto& alias : aliases) {
				if (alias != name) {
					aliases_string += fmt::format("`{}` ", alias);
				}
			}

			fields.push_back({field()
								  .name("**Aliases**")
								  .value(std::move(aliases_string))
								  .is_inline(true)});
		}
		if (!command.metadata().examples().empty()) {
			std::string examples_string;
			examples_string.reserve(command_prefix.length());

			for (const auto& example : command.metadata().examples()) {
				examples_string += fmt::format("`> {}{} {}`\n", command_prefix,
											   name, example.first);
				examples_string += fmt::format("{}\n", example.second);
			}

			fields.push_back({field()
								  .name("**Examples**")
								  .value(std::move(examples_string))});
		}

		embed.fields(std::move(fields));

		obj.channel.create_message_embed(
			aegis::create_message_t().embed(embed));

		return;
	}

	std::vector<field> fields;
	fields.reserve(1);

	auto embed = aegis::gateway::objects::embed()
					 .title("**Commands**")
					 .color(client.config().color);
	nlohmann::fifo_map<std::string, std::string> fields_content;
	std::unordered_set<std::string_view> added;

	for (const auto& command : commands) {
		// check if command is owner-only and if the user executing it is the
		// bot's owner
		if (client.config().owner_id && command.second.owner_only()) {
			if (obj.msg.get_user().get_id().get() !=
				Taiga::Util::String::string_to_number<int64_t>(
					client.config().owner_id.value())) {
				// skip command if the IDs don't match
				continue;
			}
		}

		const auto& command_name = command.second.name();
		if (!added.count(command_name)) {
			added.insert(command_name);
			fields_content[command.second.category().name()] +=
				fmt::format("`{}` ", command_name);
		}
	}
	for (const auto& [category, content] : fields_content) {
		fields.push_back(
			field().name(std::move(category)).value(std::move(content)));
	}

	embed.fields(std::move(fields));
	obj.channel.create_message(aegis::create_message_t().embed(embed));
}

// totally not stolen from aegisbot... shh..
static void info(aegis::gateway::events::message_create& obj,
				 Taiga::Bot& client, const std::deque<std::string>&,
				 const std::string_view) {
	using aegis::gateway::objects::field;
	const auto& bot_avatar = client.core().self()->get_avatar();

	auto embed =
		aegis::gateway::objects::embed()
			.title(fmt::format("{} v{}", client.config().name, TAIGA_VERSION))
			.description(
				fmt::format("{}\nMemory usage: **{}**",
							client.config().git_repo.has_value()
								? fmt::format("[Source code]({})\n",
											  client.config().git_repo.value())
								: "",
							Taiga::Util::Various::format_bytes(
								aegis::utility::getCurrentRSS())))
			.color(client.config().color)
			.thumbnail(aegis::gateway::objects::thumbnail{fmt::format(
				"https://cdn.discordapp.com/avatars/{}/{}.webp?size=1024",
				client.core().get_id(), bot_avatar)});
	embed.fields(
		{field()
			 .name("Members")
			 .value(fmt::format("{}", client.core().get_member_count()))
			 .is_inline(true),
		 field()
			 .name("Guilds")
			 .value(fmt::format("{}", client.core().get_guild_count()))
			 .is_inline(true),
		 field()
			 .name("Channels")
			 .value(fmt::format("{}", client.core().get_channel_count()))
			 .is_inline(true),
		 field()
			 .name("Uptime")
			 .value(client.core().uptime_str())
			 .is_inline(true),
		 field()
			 .name("Miscellaneous")
			 .value(
				 fmt::format("I am shard **#{}** of **{}** running on **{}**",
							 client.core()
								 .get_shard_by_guild(obj.channel.get_guild())
								 .get_id(),
							 client.core().shard_max_count,
							 aegis::utility::platform::get_platform()))
			 .is_inline(true)});

	auto footer = aegis::gateway::objects::footer(
		fmt::format("Written in C++{} using {}, aisaka v{}", CXX_VERSION,
					AEGIS_VERSION_TEXT, AISAKA_VERSION));
	footer.icon_url =
		"https://cdn.discordapp.com/emojis/289276304564420608.png";
	embed.footer(std::move(footer));

	obj.channel.create_message_embed(aegis::create_message_t().embed(embed));
}

static void server(aegis::gateway::events::message_create& obj, Taiga::Bot&,
				   const std::deque<std::string>&, const std::string_view) {
	using aegis::gateway::objects::field;

	const auto& guild = obj.msg.get_guild();
	const auto& owner = *guild.find_member(guild.get_owner());

	std::mt19937 rand(static_cast<unsigned long>(obj.msg.get_id().get()));

	auto embed =
		aegis::gateway::objects::embed()
			.title(guild.get_name())
			.color(rand() % 0xFFFFFF)
			.thumbnail(aegis::gateway::objects::thumbnail{fmt::format(
				"https://cdn.discordapp.com/icons/{}/{}.webp?size=1024",
				guild.get_id().get(), guild.get_icon())});
	embed.fields(
		{field()
			 .name("Members")
			 .value(fmt::format("{}", guild.get_member_count()))
			 .is_inline(true),
		 field()
			 .name("Channels")
			 .value(fmt::format("{}", guild.get_channels().size()))
			 .is_inline(true),
		 field().name("Owner").value(owner.get_full_name()).is_inline(true),
		 field().name("Region").value(guild.get_region()).is_inline(true),
		 field()
			 .name("ID")
			 .value(fmt::format("{}", guild.get_id().get()))
			 .is_inline(true)});

	const auto created_at =
		std::chrono::milliseconds{aegis::snowflake::c_get_time(guild.get_id())};
	const auto footer = aegis::gateway::objects::footer(fmt::format(
		"Created on {}",
		date::format("%F at %X", std::chrono::system_clock::time_point{
									 std::move(created_at)})));
	embed.footer(std::move(footer));

	obj.channel.create_message_embed(aegis::create_message_t().embed(embed));
}

void Taiga::Categories::General::init(spdlog::logger& log,
									  Aisaka::Commands<Taiga::Bot>& commands) {
	using Command = Aisaka::Command<Taiga::Bot>;
	using Metadata = Aisaka::Metadata;
	using Parameter = Aisaka::Parameter;

	commands.add_command(
		Command("help")
			.category(*this)
			.metadata(
				Metadata()
					.description("The command you're looking at right now.")
					.examples(Examples{
						{"", "Lists all commands."},
						{"help", "Sends help for the `help` command."},
						{"General", "Sends help for the `General` category."},
						{"weather category",
						 "Sends help for the `weather` **category**. "
						 "(note the explicitly stated type!)"},
						{"weather command",
						 "Sends help for the `weather` **command**. (note the "
						 "explicitly stated type!)"}}))
			.params({Parameter("name").required(false),
					 Parameter("type").required(false)})
			.function(help),
		log);
	commands.add_command(  //
		Command("info")
			.category(*this)
			.metadata(Metadata().description("Bot info."))
			.function(info),
		log);
	commands.add_command(  //
		Command("server")
			.category(*this)
			.metadata(Metadata().description("Server info."))
			.function(server),
		log);
	commands.add_command(
		Command("invite")
			.function(
				[](const auto& obj, auto& client, const auto&, const auto&) {
					obj.channel.create_message(fmt::format(
						"https://discordapp.com/oauth2/"
						"authorize?client_id={}&scope=bot&permissions=270400",
						client.core().get_id()));
				})
			.category(*this),
		log);
}
