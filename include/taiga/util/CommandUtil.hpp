#ifndef MATHUTIL_HPP
#define MATHUTIL_HPP

#include <aegis.hpp>

namespace Taiga::Util::Command {
std::optional<aegis::user*> find_user(const std::string&,
									  const aegis::gateway::objects::message&,
									  Taiga::Client&);
}  // namespace Taiga::Util::Command

#endif