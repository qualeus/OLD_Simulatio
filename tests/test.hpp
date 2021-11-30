// clang-format off
#include <stdexcept>
#include <string>
#include "../include/Geometry/Maths.hpp"
#include "../include/Geometry/String.hpp"

#define ASSERT_THROW(condition) { if (!(condition)) { throw std::runtime_error(std::string("\n") + std::string(__FILE__) + std::string(":") + std::to_string(__LINE__) + std::string(" in ") + std::string(__PRETTY_FUNCTION__)); } }

#define ASSERT_EQUAL(x, y) { if ((gmt::to_string(x)) != (gmt::to_string(y))) { throw std::runtime_error(std::string("\n") + std::string(__FILE__) + std::string(":") + std::to_string(__LINE__) + std::string(" in ") + std::string(__PRETTY_FUNCTION__) + std::string("\n[Assertion Failed]\nExpected ") + gmt::to_string(x) + std::string("\nResult ") + gmt::to_string(y)); } }
// clang-format on