#include <algorithm>
#include <taiga/util/String.hpp>

std::string Taiga::Util::String::to_upper(const std::string_view& source) {
	std::string string_upper{source};
	std::transform(string_upper.begin(), string_upper.end(),
				   string_upper.begin(),
				   [](unsigned char c) { return std::toupper(c); });

	return string_upper;
}