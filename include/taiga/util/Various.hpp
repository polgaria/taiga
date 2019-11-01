#pragma once

#include <aegis.hpp>
#include <optional>

namespace Taiga::Util {
class Various {
   public:
	static std::string format_bytes(uint64_t size);

	static float conversion_rate(const std::string_view from,
								 const std::string_view to,
								 const std::string_view api_key,
								 aegis::rest::rest_controller& rc);
};
}  // namespace Taiga::Util
