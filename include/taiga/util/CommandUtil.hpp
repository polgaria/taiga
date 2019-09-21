#ifndef COMMANDUTIL_HPP
#define COMMANDUTIL_HPP

#include <aegis.hpp>

namespace Taiga::Util::Command {
std::optional<std::reference_wrapper<aegis::user>> find_user(
	const std::string&,
	const aegis::gateway::objects::message&,
	Taiga::Client&);
}  // namespace Taiga::Util::Command

#endif