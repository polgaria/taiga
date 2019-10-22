#pragma once

#include <optional>
#include <sstream>

namespace Taiga::Util {
class String {
   public:
	template <typename T, typename = typename std::enable_if<
							  std::is_arithmetic<T>::value, T>::type>
	static std::optional<T> string_to_number(
		const std::string_view number_as_string) {
		T number;

		std::stringstream stream(number_as_string.data());
		stream >> number;

		if (stream.fail()) {
			return std::nullopt;
		}
		return number;
	}

	template <typename T, typename = typename std::enable_if<
							  std::is_arithmetic<T>::value, T>::type>
	static std::optional<T> string_to_hex(const std::string_view source) {
		T hex;
		std::stringstream stream;
		stream << std::hex << source;
		stream >> hex;

		if (stream.fail()) {
			return std::nullopt;
		}
		return hex;
	}

	static std::string to_upper(const std::string_view);
};
}  // namespace Taiga::Util
