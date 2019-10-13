#include <taiga/categories/Reddit.hpp>
#include <taiga/util/Various.hpp>

void Taiga::Categories::Reddit::init(
	spdlog::logger& log, Aisaka::Commands<Taiga::Client>& commands) {
	using Command = Aisaka::Command<Taiga::Client>;
	using Metadata = Aisaka::Metadata;

	commands.add_command(
		Command("taiga")
			.metadata(
				Metadata().description("Sends a random image from r/taiga"))
			.function([](const auto& obj, const auto&, const auto&,
						 const auto&) {
				obj.channel.create_message(
					Taiga::Util::Various::get_random_reddit_post_url("taiga"));
			})
			.category(*this),
		log);
	commands.add_command(
		Command("toradora")
			.metadata(
				Metadata().description("Sends a random image from r/toradora"))
			.function(
				[](const auto& obj, const auto&, const auto&, const auto&) {
					obj.channel.create_message(
						Taiga::Util::Various::get_random_reddit_post_url(
							"toradora"));
				})
			.category(*this),
		log);
}
