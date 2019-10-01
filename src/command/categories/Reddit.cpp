#include <aegis/channel.hpp>
#include <aegis/gateway/events/message_create.hpp>
#include <taiga/command/Commands.hpp>
#include <taiga/command/categories/Reddit.hpp>
#include <taiga/util/Various.hpp>

Taiga::Categories::Reddit::Reddit(const std::string& _name)
	: Taiga::Category(_name) {}

void Taiga::Categories::Reddit::init(spdlog::logger& log) {
	using Command = Taiga::Commands::Command;
	using Metadata = Taiga::Commands::Metadata;

	Taiga::Commands::add_command(  //
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
	Taiga::Commands::add_command(
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
