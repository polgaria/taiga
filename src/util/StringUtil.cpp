#include <algorithm>
#include <sstream>
#include <taiga/util/StringUtil.hpp>

std::deque<std::string> Taiga::Util::String::split(const std::string& source,
												   const char& delim) {
	std::stringstream ss(source);
	std::string item;
	std::deque<std::string> target;
	while (std::getline(ss, item, delim)) {
		if (!item.empty()) {
			target.push_back(item);
		}
	}

	return target;
}

std::deque<std::string> Taiga::Util::String::split_command(
	const std::string& source,
	const std::string& prefix) {
	auto arguments = Taiga::Util::String::split(source, ' ');
	arguments.push_front(prefix);

	return arguments;
}

std::string Taiga::Util::String::to_upper(const std::string& source) {
	auto string_upper{source};
	std::transform(string_upper.begin(), string_upper.end(),
				   string_upper.begin(),
				   [](unsigned char c) { return std::towupper(c); });

	return string_upper;
}

std::string Taiga::Util::String::to_lower(const std::string& source) {
	auto string_lower{source};
	std::transform(string_lower.begin(), string_lower.end(),
				   string_lower.begin(),
				   [](unsigned char c) { return std::towlower(c); });

	return string_lower;
}