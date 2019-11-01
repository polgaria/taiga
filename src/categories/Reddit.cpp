#include <cpr/cpr.h>

#include <taiga/categories/Reddit.hpp>

static std::string get_random_reddit_post_url(
	const std::string_view subreddit) {
	auto post_json_string =
		// why not aegis::rest::rest_controller? it can't handle
		// redirects; therefore, cpr it is (as much as i don't want to use it)
		cpr::Get(cpr::Url{fmt::format("https://old.reddit.com/r/{}/random.json",
									  subreddit)},
				 cpr::Header{{"User-Agent", "taiga"}})
			.text;

	auto post_json = nlohmann::json::parse(std::move(post_json_string));

	while (post_json.is_null() ||
		   post_json[0]["data"]["children"][0]["data"]["is_self"].get<bool>()) {
		post_json = get_random_reddit_post_url(subreddit);
	}

	return post_json[0]["data"]["children"][0]["data"]["url"]
		.get<std::string>();
}

void Taiga::Categories::Reddit::init(spdlog::logger& log,
									 Aisaka::Commands<Taiga::Bot>& commands) {
	using Command = Aisaka::Command<Taiga::Bot>;
	using Metadata = Aisaka::Metadata;

	commands.add_command(
		Command("taiga")
			.metadata(
				Metadata().description("Sends a random image from r/taiga"))
			.function([](const auto& obj, const auto&, const auto&,
						 const auto&) {
				obj.channel.create_message(get_random_reddit_post_url("taiga"));
			})
			.category(*this),
		log);
	commands.add_command(  //
		Command("toradora")
			.metadata(
				Metadata().description("Sends a random image from r/toradora"))
			.function(
				[](const auto& obj, const auto&, const auto&, const auto&) {
					obj.channel.create_message(
						get_random_reddit_post_url("toradora"));
				})
			.category(*this),
		log);
}
