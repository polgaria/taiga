#pragma once

#include <math.h>

namespace Taiga::Util {
class Math {
   public:
	template <typename T, typename = typename std::enable_if<
							  std::is_arithmetic<T>::value, T>::type>
	static T round_to_dec_places(T&& nonrounded, int dec_places) {
		return std::round(nonrounded * std::pow(10, dec_places) /
						  std::pow(10, dec_places));
	}
};
}  // namespace Taiga::Util
