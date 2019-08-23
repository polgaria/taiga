#ifndef UTIL_HPP
#define UTIL_HPP

#include <rapidjson/document.h>
#include <iostream>

namespace TaigaBot::Util {
rapidjson::Document get_post(const char *url);
}  // namespace TaigaBot::Util

#endif
