#include <taiga/command/categories/Category.hpp>

Taiga::Category::Category(std::string name) : name(std::move(name)) {}
Taiga::Category::~Category() = default;
