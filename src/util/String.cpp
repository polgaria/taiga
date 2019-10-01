#include <algorithm>
#include <sstream>
#include <taiga/util/String.hpp>

std::deque<std::string> Taiga::Util::String::split(const std::string& source,
												   const char& delim) {
	std::deque<std::string> tokens;
	std::size_t start = 0, end = 0;
	while ((end = source.find(delim, start)) != std::string::npos) {
		tokens.push_back(source.substr(start, end - start));
		start = end + 1;
	}
	tokens.push_back(source.substr(start));
	return tokens;
}

std::deque<std::string> Taiga::Util::String::split_command(
	const std::string& source, const std::string& prefix) {
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