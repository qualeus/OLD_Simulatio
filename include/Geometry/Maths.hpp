#ifndef Maths_HPP
#define Maths_HPP

#define PI 3.141592653589793  /* Archimede Constant */
#define EU 2.718281828459045  /* Euler Number */
#define TAU 6.283185307179586 /* Tau as 2PI */
#define GR 1.618033988749894  /* Golder Ratio */
#define PC 1.414213562373095  /* Pythagoras Constant */

#define RO 360 /* Complete rotation */

#include <SFML/Graphics.hpp>  // To delete when the transition to VectorI will be done
#include <cmath>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

namespace phy {
class Circle;
class Corpse;
class Polygon;
}  // namespace phy

namespace gmt {

template <typename T>
class Vector;

template <typename T>
class Bounds;

template <typename T>
class Quadtree;

template <typename T>
class Collision;

template <typename T>
class Text;

bool float_equals(float a, float b, float sigma = 0.1f);
float degree_to_radian(float degree);
float radian_to_degree(float radian);
float digits_comma(float number, int digits);

template <class C>
std::shared_ptr<C> remove(int i, std::vector<std::shared_ptr<C>> &vect) {
    std::shared_ptr<C> corpse = vect.at(i);
    vect.erase(vect.begin() + i);
    return corpse;
}

template <typename T>
std::string to_string(T value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

template <typename T>
std::string to_string(std::vector<T> vector) {
    std::ostringstream oss;
    oss << "{";
    if (!vector.empty()) {
        std::copy(vector.begin(), vector.end() - 1, std::ostream_iterator<T>(oss, ","));
        oss << vector.back();
    }
    oss << "}";
    return oss.str();
}

template <typename T>
std::string to_string(sf::Vector2<T> vector) {
    std::ostringstream oss;
    oss << "{" << vector.x << ";" << vector.y << "}";
    return oss.str();
}

template <typename T>
bool decimal_equals(const T &a, const T &b, T epsilon = std::numeric_limits<T>::epsilon()) {
    return fabs(a - b) < epsilon;
}
}  // namespace gmt
#endif