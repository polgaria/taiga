#include <taiga/command/categories/Category.hpp>

Taiga::Command::Category::Category(std::string name) : name(std::move(name)) {}
Taiga::Command::Category::~Category() = default;
