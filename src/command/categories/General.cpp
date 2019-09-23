#include <date/date.h>
#include <aegis.hpp>
#include <iostream>
#include <taiga/command/Command.hpp>
#include <taiga/command/categories/General.hpp>

Taiga::Command::Categories::General::General(const std::string& _name)
	: Taiga::Command::Category(_name) {}

COMMAND(help) {
	using aegis::gateway::objects::field;

	if (!params.empty()) {
		const auto& found_command = Taiga::Command::all.find(params.front());
		if (found_command == Taiga::Command::all.end()) {
			obj.channel.create_message("Command not found.");
			return;
		}

		const auto& command = Taiga::Command::all.at(params.front());
		auto fields = std::vector<field>();

		auto embed{aegis::gateway::objects::embed()
					   .title(fmt::format("**{}**", command.name))
					   .color(0x3498DB)};

		auto syntax{fmt::format(command.params.empty() ? "`{}{}" : "`{}{} ",
								client.get_config().prefix, command.name)};
		for (const auto& param : command.params) {
			syntax +=
				fmt::format(param.required ? "<{}> " : "[{}] ", param.name);
		}
		syntax += '`';

		fields.push_back({field().name("**Syntax**").value(syntax)});

		if (command.description) {
			fields.push_back({field()
								  .name("**Description**")
								  .value(command.description.value())});
		}

		embed.fields(fields);

		obj.channel.create_message_embed({}, embed);

		return;
	}

	auto embed{
		aegis::gateway::objects::embed().title("**Commands**").color(0x3498DB)};
	auto fields_content = nlohmann::fifo_map<std::string, std::string>();
	auto fields = std::vector<field>();

	for (const auto& command : Taiga::Command::all) {
		// should i be doing this?? there's probably a faster way.. eh whatever
		fields_content[command.second.category] +=
			fmt::format("`{}` ", command.first);
	}
	for (const auto& [category, content] : fields_content) {
		fields.push_back(field().name(category).value(content));
	}

	embed.fields(fields);
	obj.channel.create_message(aegis::create_message_t().embed(embed));
}

// totally not stolen from aegisbot... shh..
COMMAND(info) {
	using aegis::gateway::objects::field;

	const auto& bot_avatar =
		client.get_bot()->find_user(client.get_bot()->get_id())->get_avatar();

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
				client.get_bot()->get_id().get(), bot_avatar)});
	embed.fields(
		{field()
			 .name("Members")
			 .value(fmt::format("{}", client.get_bot()->get_member_count()))
			 .is_inline(true),
		 field()
			 .name("Guilds")
			 .value(fmt::format("{}", client.get_bot()->get_guild_count()))
			 .is_inline(true),
		 field()
			 .name("Channels")
			 .value(fmt::format("{}", client.get_bot()->get_channel_count()))
			 .is_inline(true),
		 field()
			 .name("Uptime")
			 .value(client.get_bot()->uptime_str())
			 .is_inline(true),
		 field()
			 .name("Miscellaneous")
			 .value(
				 fmt::format("I am shard **#{}** of **{}** running on **{}**",
							 client.get_bot()
								 ->get_shard_by_guild(obj.channel.get_guild())
								 .get_id(),
							 client.get_bot()->shard_max_count,
							 aegis::utility::platform::get_platform()))
			 .is_inline(true)});

	auto footer = aegis::gateway::objects::footer(
		fmt::format("Made in C++{} using {}", CXX_VERSION, AEGIS_VERSION_TEXT));
	footer.icon_url =
		"https://cdn.discordapp.com/emojis/289276304564420608.png";
	embed.footer(footer);

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
		 field().name("Owner").value(owner.get_full_name()).is_inline(true)});

	const auto created_at =
		std::chrono::milliseconds{aegis::snowflake::c_get_time(guild.get_id())};
	const auto footer = aegis::gateway::objects::footer(fmt::format(
		"Created on {}",
		date::format("%F at %X",
					 std::chrono::system_clock::time_point{created_at})));
	embed.footer(footer);

	obj.channel.create_message_embed(aegis::create_message_t().embed(embed));
}

void Taiga::Command::Categories::General::init(spdlog::logger& log) {
	ADD_COMMAND_DESC(help, "The command you're looking at right now.",
					 {{"command", false}});
	ADD_COMMAND_DESC(info, "Bot info.", {});
	ADD_COMMAND_DESC(server, "Server info.", {});
}
