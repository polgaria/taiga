#ifndef GENERAL_HPP
#define GENERAL_HPP

#include <taiga/command/categories/Category.hpp>

namespace Taiga::Command::Categories {
class General : Taiga::Command::Category {
   public:
	General(const std::string&);
	void init(spdlog::logger& log) override;
};
}  // namespace Taiga::Command::Categories

#endif
