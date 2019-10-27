#include <taiga/util/Date.hpp>

template <typename T>
using Duration = std::chrono::duration<float, T>;

static std::optional<date::sys_days> parse_date(const std::string& str) {
	std::istringstream in(str);
	date::sys_days tp;
	// 2019-01-01
	in >> date::parse("%F", tp);
	if (!in.fail()) {
		return tp;
	}
	return std::nullopt;
}

std::tuple<float, float, float> Taiga::Util::Date::time_since_date(
	const date::sys_seconds& date) {
	const auto& d_since_date = std::chrono::system_clock::now() - date;

	const auto& days_since_date =
		Duration<date::days::period>(d_since_date).count();
	const auto& months_since_date =
		Duration<date::months::period>(d_since_date).count();
	const auto& years_since_date =
		Duration<date::years::period>(d_since_date).count();

	return std::make_tuple(days_since_date, months_since_date,
						   years_since_date);
}

std::tuple<float, float, float> Taiga::Util::Date::time_since_date(
	const std::string& str_date) {
	const auto& date = parse_date(str_date);
	if (!date) {
		throw std::invalid_argument("Invalid date.");
	}
	return time_since_date(date.value());
}