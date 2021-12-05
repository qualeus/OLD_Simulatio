// clang-format off
#include <stdexcept>
#include <string>
#include "../include/Geometry/Maths.hpp"
#include "../include/Geometry/String.hpp"
#include "../include/Geometry/Vector.hpp"
#include "../include/Geometry/Vertices.hpp"
#include "../include/Geometry/Bounds.hpp"

template <class T>
using Unit = T;

template <class T>
using Vect = std::vector<T>;

template <class T>
using Pair = std::vector<std::pair<T, T>>;

template <class T>
using SVect = std::vector<std::shared_ptr<T>>;

template <class T>
using SPair = std::vector<std::pair<std::shared_ptr<T>, std::shared_ptr<T>>>;

#define ASSERT_THROW(condition) { if (!(condition)) { throw std::runtime_error(std::string("\n") + std::string(__FILE__) + std::string(":") + std::to_string(__LINE__) + std::string(" in ") + std::string(__PRETTY_FUNCTION__)); } }

#define ASSERT_EQUAL(x, y) { if ((gmt::to_string(x)) != (gmt::to_string(y))) { throw std::runtime_error(std::string("\n") + std::string(__FILE__) + std::string(":") + std::to_string(__LINE__) + std::string(" in ") + std::string(__PRETTY_FUNCTION__) + std::string("\n[Assertion Failed]\nExpected ") + gmt::to_string(x) + std::string("\nResult ") + gmt::to_string(y)); } }
// clang-format on