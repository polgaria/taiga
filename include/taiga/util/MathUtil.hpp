#ifndef MATHUTIL_HPP
#define MATHUTIL_HPP

#include <math.h>

namespace Taiga::Util::Math {
template <
	typename T,
	typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
T round_to_dec_places(T nonrounded, int dec_places) {
	return std::round(nonrounded * (10 ^ dec_places)) / (10 ^ dec_places);
}
}  // namespace Taiga::Util::Math

#endif