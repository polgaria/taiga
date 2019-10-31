#pragma once

#include <math.h>

namespace Taiga::Util {
class Math {
   public:
	template <typename T, typename = typename std::enable_if<
							  std::is_arithmetic<T>::value, T>::type>
	static T round_to_dec_places(const T&& nonrounded, ushort dec_places) {
		const auto& mul = std::pow(10, dec_places);
		return std::round(nonrounded * mul) / mul;
	}
};
}  // namespace Taiga::Util