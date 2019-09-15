#ifndef STRINGUTIL_HPP
#define STRINGUTIL_HPP

#include <spdlog/fmt/bundled/format.h>
#include <deque>
#include <optional>

namespace Taiga::Util::String {
template <
	typename T,
	typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
std::optional<T> string_to_number(const std::string& number_as_string) {
	T valor;

	std::stringstream stream(number_as_string);
	stream >> valor;
	if (stream.fail()) {
		return std::nullopt;
	}
	return valor;
}
std::deque<std::string> split_by_space(const std::string&);
std::deque<std::string> split_command(const std::string& source,
									  const std::string& prefix);
std::string to_upper(const std::string&);
std::string to_lower(const std::string&);
}  // namespace Taiga::Util::String

#endif
