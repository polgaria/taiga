#include <date/date.h>
#include <aegis.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <iostream>
#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>
#include <taiga/command/Command.hpp>
#include <taiga/command/categories/General.hpp>
#include <taiga/util/Command.hpp>
#include <taiga/util/String.hpp>

Taiga::Command::Categories::General::General(const std::string& _name)
	: Taiga::Command::Category(_name) {}

COMMAND(help) {
	using aegis::gateway::objects::field;
	auto fields = std::vector<field>();

	if (!params.empty()) {
		const auto& command_name =
			Taiga::Util::String::to_lower(params.front());
		const auto& found_command = Taiga::Commands::all.find(command_name);
		if (found_command == Taiga::Commands::all.end()) {
			obj.channel.create_message("Command not found.");
			return;
		}

		auto& command = Taiga::Commands::all[command_name];

		auto embed{aegis::gateway::objects::embed()
					   .title(fmt::format("**{}**", command_name))
					   .color(0x3498DB)};

		auto syntax{fmt::format(command.params().empty() ? "`{}{}" : "`{}{} ",
								command_prefix, command_name)};
		for (const auto& param : command.params()) {
			syntax +=
				fmt::format(param.required ? "<{}> " : "[{}] ", param.name);
		}
		syntax += '`';

		fields.push_back({field()
							  .name("**Syntax**")
							  .value(std::move(syntax))
							  .is_inline(true)});

		if (!command.description().empty()) {
			embed.description(command.description());
		}
		if (!command.aliases().empty()) {
			auto& aliases = command.aliases();

			std::string aliases_string{};

			// if the command name used is an alias, add the actual command name
			if (aliases.find(command_name) != aliases.end()) {
				aliases_string += fmt::format("`{}` ", command.name());
			}
			for (const auto& alias : aliases) {
				if (alias != command_name) {
					aliases_string += fmt::format("`{}` ", alias);
				}
			}

			fields.push_back({field()
								  .name("**Aliases**")
								  .value(std::move(aliases_string))
								  .is_inline(true)});
		}

		embed.fields(std::move(fields));

		obj.channel.create_message_embed(
			aegis::create_message_t().embed(embed));

		return;
	}

	auto embed =
		aegis::gateway::objects::embed().title("**Commands**").color(0x3498DB);
	auto fields_content = nlohmann::fifo_map<std::string, std::string>();
	auto added = std::unordered_map<std::string, bool>();

	for (auto& command : Taiga::Commands::all) {
		// check if command is owner-only and if the user executing it is the
		// bot's owner
		if (client.get_config().owner_id && command.second.owner_only()) {
			if (obj.msg.get_user().get_id().get() !=
				Taiga::Util::String::string_to_number<int64_t>(
					client.get_config().owner_id.value())) {
				// skip command if the IDs don't match
				continue;
			}
		}

		const auto& command_name = command.second.name();
		if (!added.count(command_name)) {
			added[command_name] = true;
			fields_content[command.second.category()] +=
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
COMMAND(info) {
	using aegis::gateway::objects::field;

	const auto& bot_avatar = obj.msg.get_guild().self()->get_avatar();

	std::mt19937 rand(static_cast<unsigned long>(obj.msg.get_id().get()));

	auto embed =
		aegis::gateway::objects::embed()
			.title(client.get_config().name)
			.description(fmt::format(
				"{}\nMemory usage: **{}MB**",
				client.get_config().git_repo.has_value()
					? fmt::format("[Source code]({})\n",
								  client.get_config().git_repo.value())
					: "",
				aegis::utility::getCurrentRSS() / (1024 * 1024)))
			.color(rand() % 0xFFFFFF)
			.thumbnail(aegis::gateway::objects::thumbnail{fmt::format(
				"https://cdn.discordapp.com/avatars/{}/{}.webp?size=1024",
				client.get_bot().get_id().get(), bot_avatar)});
	embed.fields(
		{field()
			 .name("Members")
			 .value(fmt::format("{}", client.get_bot().get_member_count()))
			 .is_inline(true),
		 field()
			 .name("Guilds")
			 .value(fmt::format("{}", client.get_bot().get_guild_count()))
			 .is_inline(true),
		 field()
			 .name("Channels")
			 .value(fmt::format("{}", client.get_bot().get_channel_count()))
			 .is_inline(true),
		 field()
			 .name("Uptime")
			 .value(client.get_bot().uptime_str())
			 .is_inline(true),
		 field()
			 .name("Miscellaneous")
			 .value(
				 fmt::format("I am shard **#{}** of **{}** running on **{}**",
							 client.get_bot()
								 .get_shard_by_guild(obj.channel.get_guild())
								 .get_id(),
							 client.get_bot().shard_max_count,
							 aegis::utility::platform::get_platform()))
			 .is_inline(true)});

	auto footer = aegis::gateway::objects::footer(
		fmt::format("Made in C++{} using {}", CXX_VERSION, AEGIS_VERSION_TEXT));
	footer.icon_url =
		"https://cdn.discordapp.com/emojis/289276304564420608.png";
	embed.footer(std::move(footer));

	obj.channel.create_message_embed(aegis::create_message_t().embed(embed));
}

COMMAND(server) {
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

// WIP
COMMAND(user) {
	using aegis::gateway::objects::field;

	const auto& _user =
		Taiga::Util::Command::find_user(params.front(), obj.msg, client);
	if (!_user) {
		obj.channel.create_message("User not found!");
	}
	const auto& user = _user.value().get();

	std::mt19937 rand(static_cast<unsigned long>(obj.msg.get_id().get()));

	auto embed =
		aegis::gateway::objects::embed()
			.title(user.get_full_name())
			.color(rand() % 0xFFFFFF)
			.thumbnail(aegis::gateway::objects::thumbnail{fmt::format(
				"https://cdn.discordapp.com/avatars/{}/{}.webp?size=1024",
				user.get_id().get(), user.get_avatar())});

	const auto created_at =
		std::chrono::milliseconds{aegis::snowflake::c_get_time(user.get_id())};
	const auto footer = aegis::gateway::objects::footer(fmt::format(
		"Created on {}",
		date::format("%F at %X",
					 std::chrono::system_clock::time_point{created_at})));
	embed.footer(footer);

	obj.channel.create_message_embed(aegis::create_message_t().embed(embed));
}

// TODO: clean up this horrible mess
COMMAND(_prefix) {
	using bsoncxx::builder::stream::close_document;
	using bsoncxx::builder::stream::document;
	using bsoncxx::builder::stream::finalize;
	using bsoncxx::builder::stream::open_document;

	const auto& guild = obj.msg.get_guild();
	// TODO: handle this in Client::on_message, make it a command trait
	const aegis::permission& user_perms =
		guild.base_permissions(obj.msg.get_user());
	const auto& mode = params.front();
	if (!user_perms.can_manage_messages() && mode != "list") {
		obj.channel.create_message(
			"You're missing the `Manage Messages` permission!");
		return;
	}

	const auto& mongo_client = client.get_mongo_pool().acquire();
	const auto& db = (*mongo_client)[client.get_config().name];
	auto prefixes = db["prefixes"];

	const bsoncxx::document::view_or_value& guild_document =
		document{} << "id" << guild.get_id() << finalize;

	if (mode == "remove" || mode == "delete") {
		if (params.size() == 1) {
			obj.channel.create_message("Too few parameters.");
			return;
		}
		const auto& prefix = params[1];

		// check if there's only one prefix
		const auto& has_one_prefix = prefixes.find_one(
			// clang-format off
			document{}
					<< "id" << guild.get_id()
					<< "prefix" << open_document
					   << "$size" << 1
					<< close_document
					<< finalize
			// clang-format on
		);
		if (has_one_prefix) {
			prefixes.delete_one(guild_document);
			obj.channel.create_message("Reset to default prefix.");
			return;
		}

		const auto& delete_prefix = prefixes.update_one(
			guild_document,
			// clang-format off
			document{}
				<< "$pull" << open_document
					<< "prefix" << prefix
				<< close_document
				<< finalize
			// clang-format on
		);

		// check if there's only one prefix and it's the default prefix
		const auto& has_one_prefix_default = prefixes.find_one(
			// clang-format off
					document{}
							<< "id" << guild.get_id()
							<< "prefix" << open_document
							   << "$size" << 1
							<< close_document
							<< "prefix" << client.get_config().prefix
							<< finalize
			// clang-format on
		);
		// wow
		if (has_one_prefix_default) {
			prefixes.delete_one(guild_document);
			obj.channel.create_message("Reset to default prefix.");
			return;
		}

		// if no prefixes were deleted
		if (delete_prefix->modified_count() == 0) {
			obj.channel.create_message("Prefix not found!");
		} else {
			obj.channel.create_message(
				fmt::format("Removed prefix `{}`.", prefix));
		}
		return;
	} else if (mode == "add") {
		if (params.size() == 1) {
			obj.channel.create_message("Too few parameters.");
			return;
		}
		const auto& prefix = params[1];

		if (const auto& prefix_exists =
				prefixes.find_one(document{} << "id" << guild.get_id()
											 << "prefix" << prefix << finalize);
			prefix_exists) {
			obj.channel.create_message("Prefix already added!");
			return;
		}

		// add the default prefix as well if there are currently no custom
		// prefixes
		if (const auto& document_exists = prefixes.find_one(guild_document);
			!document_exists) {
			// clang-format off
			prefixes.update_one(guild_document,
								document{}
								<< "$push" << open_document
									<< "prefix" << client.get_config().prefix
								<< close_document
								<< finalize,
								mongocxx::options::update{}.upsert(true));
			// clang-format on
		}
		// clang-format off
		prefixes.update_one(guild_document,
							document{}
                            << "$push" << open_document
                                << "prefix" << prefix
                            << close_document
                            << finalize,
							mongocxx::options::update{}.upsert(true));
		// clang-format on
		obj.channel.create_message(fmt::format(
			"`{}` has been added to the server's prefixes.", prefix));
	} else if (mode == "list") {
		const auto& guild_prefixes = prefixes.find_one(guild_document);
		if (!guild_prefixes) {
			obj.channel.create_message("This server has no custom prefixes!");
			return;
		}
		std::string output{};
		// iterate through guild prefixes
		for (const auto& res :
			 guild_prefixes->view()["prefix"].get_array().value) {
			const auto& _prefix = std::string{res.get_utf8().value};
			output += fmt::format("`{}`\n", _prefix);
		}
		obj.channel.create_message(output);
	} else {
		obj.channel.create_message("Invalid mode!");
	}
}

COMMAND(invite) {
	obj.channel.create_message(
		fmt::format("https://discordapp.com/oauth2/"
					"authorize?client_id={}&scope=bot&permissions=270400",
					client.get_bot().get_id().get()));
}

void Taiga::Command::Categories::General::init(spdlog::logger& log) {
	Taiga::Commands::add_command(
		Taiga::Commands::Command()
			.name("help")
			.category(std::forward<std::string>(this->get_name()))
			.description("The command you're looking at right now.")
			.params({{"command", false}})
			.function(help),
		log);
	Taiga::Commands::add_command(  //
		Taiga::Commands::Command()
			.name("info")
			.category(std::forward<std::string>(this->get_name()))
			.description("Bot info.")
			.function(info),
		log);
	Taiga::Commands::add_command(  //
		Taiga::Commands::Command()
			.name("server")
			.category(std::forward<std::string>(this->get_name()))
			.description("Server info.")
			.function(server),
		log);
	Taiga::Commands::add_command(  //
		Taiga::Commands::Command()
			.name("prefix")
			.category(std::forward<std::string>(this->get_name()))
			.description("Adds/removes a server-specific prefix, depending on "
						 "the mode requested.\n"
						 "Possible modes: `add`, `remove`/`delete`, `list`.\n"
						 "Requires the `Manage Messages` permission.")
			.params({{"mode"}, {"prefix", false}})
			.function(_prefix),
		log);
	// clang-format off
	Taiga::Commands::add_command(
		Taiga::Commands::Command()
			.name("invite")
			.function(invite)
			.category(std::forward<std::string>(this->get_name())),
		log);
	// clang-format on	
	/*Taiga::Commands::add_command(  //
		Taiga::Commands::Command()
			.name("user")
			.category(this->name)
			.description("User info.")
			.function(user),
		log);*/
}
