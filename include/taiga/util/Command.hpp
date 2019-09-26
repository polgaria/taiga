#ifndef UTIL_COMMAND_HPP
#define UTIL_COMMAND_HPP

#include <aegis.hpp>
#include <taiga/Client.hpp>

namespace Taiga::Util {
class Command {
   public:
	static std::optional<std::reference_wrapper<aegis::user>> find_user(
		const std::string&, const aegis::gateway::objects::message&,
		Taiga::Client&);
};
}  // namespace Taiga::Util

#endif
