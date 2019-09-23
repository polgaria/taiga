#include <aegis.hpp>
#include <taiga/command/Command.hpp>
#include <taiga/command/categories/Miscellaneous.hpp>
#include <taiga/util/StringUtil.hpp>
#include <taiga/util/Util.hpp>

Taiga::Command::Categories::Miscellaneous::Miscellaneous(
	const std::string& _name)
	: Taiga::Command::Category(_name) {}

COMMAND(rate) {
	const auto ratee = Taiga::Util::String::join(params, " ");

	const auto hash = std::hash<std::string>();
	std::mt19937 rand(hash(ratee));

	obj.channel.create_message(
		fmt::format("I'd rate {} a {}/10", ratee,
					std::uniform_int_distribution<>{0, 10}(rand)));
}

COMMAND(progress) {
	auto percent = Taiga::Util::year_progress();
	std::string msg;

	for (auto progress = 0; progress < 20; progress++) {
		msg += progress < percent / 5 ? "▓" : "░";
	}

	obj.channel.create_message(fmt::format("{} {:.2f}%", msg, percent));
}

COMMAND(kva) {
	obj.channel.create_message(
		"ква ква ква  гав гав гав    мяяяяяу   беееее  муууу  ку ку");
}

void Taiga::Command::Categories::Miscellaneous::init(spdlog::logger& log) {
	ADD_COMMAND_DESC(rate, "Rates things on a scale of 0 to 10.",
					 {{"thing to rate"}});
	ADD_COMMAND_DESC(progress, "Progress to the end of the year.", {});
	ADD_COMMAND(kva, {});
}
