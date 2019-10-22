#include <date/date.h>

#include <aisaka/util/String.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>
#include <taiga/categories/Prefix.hpp>
#include <taiga/util/String.hpp>

static void remove_prefix_from_cache(
	std::unordered_multimap<std::int64_t, std::string>& cache,
	const int64_t& id, const std::string_view& prefix) {
	const auto& range = cache.equal_range(id);

	const auto& prefix_in_cache = std::find_if(
		range.first, range.second,
		[&prefix](const auto& x) { return x.second.data() == prefix; });
	if (prefix_in_cache != cache.end()) {
		cache.erase(prefix_in_cache);
	}
}

static void add_prefix_to_cache(
	std::unordered_multimap<std::int64_t, std::string>& cache,
	const int64_t& id, const std::string_view& prefix) {
	cache.emplace(id, prefix);
}

// TODO: clean up this horrible mess
static void prefix(aegis::gateway::events::message_create& obj,
				   Taiga::Bot& client,
				   const std::deque<std::string>& params,
				   const std::string&) {
	using bsoncxx::builder::stream::close_document;
	using bsoncxx::builder::stream::document;
	using bsoncxx::builder::stream::finalize;
	using bsoncxx::builder::stream::open_document;

	const auto& guild = obj.msg.get_guild();

	// TODO: handle this in Client::on_message, make it a command trait
	const aegis::permission& user_perms =
		guild.base_permissions(obj.msg.get_user());
	// the owner should be able to add/remove prefixes no matter their
	// permissions
	const auto& user_is_owner =
		fmt::format("{}", obj.msg.get_user().get_id()) ==
		client.config().owner_id.value_or("");
	const auto& mode = params.front();
	if (!user_perms.can_manage_messages() && mode != "list" && !user_is_owner) {
		obj.channel.create_message(
			"You're missing the `Manage Messages` permission!");
		return;
	}

	const auto& mongo_client = client.mongo_pool().acquire();
	const auto& db = (*mongo_client)[client.config().name];
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
			remove_prefix_from_cache(client.prefix_cache(),
									 obj.channel.get_guild_id(), prefix);
			prefixes.delete_one(guild_document);
			obj.channel.create_message("Reset to default prefix.");
			return;
		}

		const auto& delete_prefix = prefixes.update_one(
			guild_document,
			// clang-format off
			document{}
				<< "$pull" << open_document
					<< "prefix" << prefix.data()
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
							<< "prefix" << client.config().prefix
							<< finalize
			// clang-format on
		);

		// remove from prefix cache as well!
		if (delete_prefix->modified_count() > 0 || has_one_prefix_default) {
			remove_prefix_from_cache(client.prefix_cache(),
									 obj.channel.get_guild_id(), prefix);
		}

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

		if (const auto& prefix_exists = prefixes.find_one(
				document{} << "id" << guild.get_id() << "prefix"
						   << prefix.data() << finalize);
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
									<< "prefix" << client.config().prefix
								<< close_document
								<< finalize,
								mongocxx::options::update{}.upsert(true));
			// clang-format on
		}
		// clang-format off
		prefixes.update_one(guild_document,
							document{}
                            << "$push" << open_document
                                << "prefix" << prefix.data()
                            << close_document
                            << finalize,
							mongocxx::options::update{}.upsert(true));
		
		// add to prefix cache
		add_prefix_to_cache(client.prefix_cache(), guild.get_id(), prefix);

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
			output += fmt::format("`{}`\n", res.get_utf8().value.to_string());
		}
		obj.channel.create_message(output);
	} else {
		obj.channel.create_message("Invalid mode!");
	}
}

void Taiga::Categories::Prefix::init(spdlog::logger& log,
									 Aisaka::Commands<Taiga::Bot>& commands) {
	using Command = Aisaka::Command<Taiga::Bot>;
	using Metadata = Aisaka::Metadata;
	using Parameter = Aisaka::Parameter;

	commands.add_command(  //
		Command("prefix")
			.category(*this)
			.metadata(
				Metadata()
					.description(
						"Adds/removes a server-specific prefix, depending "
						"on "
						"the mode requested.\n"
						"Possible modes: `add`, `remove`/`delete`, "
						"`list`.\n"
						"Requires the `Manage Messages` permission.")
					.examples(Examples{
						{"add test", "Adds `test` to the guild prefixes."},
						{"remove test",
						 "Removes `test` from the guild prefixes."},
						{"list", "Lists all guild prefixes."}}))
			.params({Parameter("mode"), Parameter("prefix").required(false)})
			.function(prefix),
		log);
}
