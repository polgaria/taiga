#ifndef MATHUTIL_HPP
#define MATHUTIL_HPP

#include <aegis.hpp>

namespace Taiga::Util::Command {
std::unique_ptr<aegis::user>& find_user(const std::string& name,
										const aegis::snowflake& guild_id,
										Taiga::Client* client);
}  // namespace Taiga::Util::Command

#endif