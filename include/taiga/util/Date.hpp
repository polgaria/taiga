#pragma once

#include <date/date.h>

#include <optional>

namespace Taiga::Util {
class Date {
   public:
	static std::tuple<float, float, float> time_since_date(
		const date::sys_seconds& date);

	static std::tuple<float, float, float> time_since_date(
		const std::string& str_date);

	static std::tuple<float, float, float> time_until_date(
		const date::sys_seconds& date);
};
}  // namespace Taiga::Util
