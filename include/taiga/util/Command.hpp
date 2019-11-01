#pragma once

#include <aegis.hpp>
#include <taiga/Bot.hpp>

namespace Taiga::Util {
class Command {
   public:
	static std::optional<std::reference_wrapper<aegis::user>> find_user(
		const std::string_view, aegis::gateway::objects::message&, Taiga::Bot&);
};
}  // namespace Taiga::Util
