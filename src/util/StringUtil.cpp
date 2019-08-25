#include <sstream>
#include <taigabot/util/StringUtil.hpp>

std::deque<std::string> TaigaBot::Util::String::split_command(
	const std::string& source) {
	std::stringstream ss(source);
	std::string item;
	std::deque<std::string> target;
	while (std::getline(ss, item, ' ')) {
		if (!item.empty()) {
			target.push_back(item);
		}
	}
	return target;
}

std::string& TaigaBot::Util::String::to_upper(std::string& string) {
	for (auto& c : string) {
		c = toupper(static_cast<unsigned char>(c));
	}
	return string;
}