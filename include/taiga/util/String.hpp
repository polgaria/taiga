#pragma once

#include <spdlog/fmt/bundled/format.h>
#include <deque>
#include <iterator>
#include <optional>
#include <sstream>

namespace Taiga::Util {
class String {
   public:
	template <typename T, typename = typename std::enable_if<
							  std::is_arithmetic<T>::value, T>::type>
	static std::optional<T> string_to_number(
		const std::string& number_as_string) {
		T number;

		std::stringstream stream(number_as_string);
		stream >> number;
		if (stream.fail()) {
			return std::nullopt;
		}
		return number;
	}

	// https://stackoverflow.com/questions/5288396/c-ostream-out-manipulation/5289170#5289170
	template <typename Range, typename Value = typename Range::value_type>
	static std::string join(const Range& elements,
							const std::string& delimiter) {
		std::ostringstream os;
		auto b = begin(elements);
		auto e = end(elements);

		if (b != e) {
			std::copy(b, prev(e),
					  std::ostream_iterator<Value>(os, delimiter.c_str()));
			b = prev(e);
		}
		if (b != e) {
			os << *b;
		}

		return os.str();
	}

	static std::deque<std::string> split(const std::string&, const char& delim);
	static std::deque<std::string> split_command(const std::string& source,
												 const std::string& prefix);
	static std::string to_upper(const std::string&);
	static std::string to_lower(const std::string&);
};
}  // namespace Taiga::Util
