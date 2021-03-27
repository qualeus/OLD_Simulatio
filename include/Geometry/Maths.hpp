#ifndef Maths_HPP
#define Maths_HPP

#define PI 3.141592653589793  /* Archimede Constant */
#define EU 2.718281828459045  /* Euler Number */
#define TAU 6.283185307179586 /* Tau as 2PI */
#define GR 1.618033988749894  /* Golder Ratio */
#define PC 1.414213562373095  /* Pythagoras Constant */

#define RO 360 /* Complete rotation */

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <memory>
#include <sstream>
#include <string_view>
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
std::vector<int> cyclic_indexes(int min, int max, int size);
unsigned modulo(int value, unsigned m);

template <class C>
std::shared_ptr<C> remove(int i, std::vector<std::shared_ptr<C>> &vect) {
    std::shared_ptr<C> ptr = vect.at(i);
    vect.erase(vect.begin() + i);
    return ptr;
}

template <class C>
std::pair<std::shared_ptr<C>, std::shared_ptr<C>> remove(int i, std::vector<std::pair<std::shared_ptr<C>, std::shared_ptr<C>>> &vect) {
    std::pair<std::shared_ptr<C>, std::shared_ptr<C>> ptr = vect.at(i);
    vect.erase(vect.begin() + i);
    return ptr;
}

template <class C>
void remove_pairs(int i, std::vector<std::shared_ptr<C>> &vect, std::vector<std::pair<std::shared_ptr<C>, std::shared_ptr<C>>> &pairs) {
    vect.erase(vect.begin() + i);  // Remove the object from the vector

    int first_pair = ((i - 1) * i) / 2;
    pairs.erase(pairs.begin() + first_pair, pairs.begin() + first_pair + i);  // Remove the first interval from pairs

    int next_pair = first_pair + i;
    int step = i;
    while (next_pair < pairs.size()) {
        pairs.erase(pairs.begin() + next_pair);  // Remove the rest from pairs
        next_pair = next_pair + step;
        step++;
    }
}

/* Avoid linear complexity of the erase but do not keep the order */
template <class C>
void remove_pairs_unordered(int i, std::vector<std::shared_ptr<C>> &vect, std::vector<std::pair<std::shared_ptr<C>, std::shared_ptr<C>>> &pairs) {
    if (i > pairs.size() - 1) { return; }
    std::shared_ptr<C> ptr = vect.at(i);  // Keep the pointer to the object for comparison
    vect.erase(vect.begin() + i);         // Remove the object from the vector

    int j = 0;
    int size = pairs.size();
    while (j < size) {
        if (pairs.at(j).first == ptr || pairs.at(j).second == ptr) {
            if (j != pairs.size() - 1) { pairs.at(j) = std::move(pairs.back()); }
            pairs.pop_back();
            size = pairs.size();
        } else {
            j++;
        }
    }
}

template <typename T>
bool decimal_equals(const T &a, const T &b, T epsilon = std::numeric_limits<T>::epsilon()) {
    return fabs(a - b) < epsilon;
}
}  // namespace gmt
#endif