#include <math.h>
#include <taigabot/util/MathUtil.hpp>

float TaigaBot::Util::Math::round_to_dec_places(float nonrounded,
												int dec_places) {
	return std::round(nonrounded * (10 ^ dec_places)) / (10 ^ dec_places);
}