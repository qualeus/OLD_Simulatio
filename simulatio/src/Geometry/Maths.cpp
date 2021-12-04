#include "../../include/Geometry/Maths.hpp"

/* Compare two floats with a given sigma */
bool gmt::float_equals(float a, float b, float sigma) {
    return (std::abs(b - a) < std::abs(sigma));  // return a + sigma > b && a - sigma < b;
}

/* Convert degree to radian: deg = rad * (PI/180) */
float gmt::degree_to_radian(float degree) { return degree * (PI / 180); }

/* Convert degree to radian: rad = deg * (180/PI) */
float gmt::radian_to_degree(float radian) { return radian * (180 / PI); }

/* Return the float with a designed number of digits behind the comma */
float gmt::digits_comma(float number, int digits) {
    int d_limit = std::pow(10, digits);
    return round(number * d_limit) / d_limit;
}

/* Return the positive modulo for a int */
unsigned gmt::modulo(int value, unsigned m) {
    int mod = value % (int)m;
    if (mod < 0) { mod += m; }
    return mod;
}

int gmt::rand_interval(int value) { return gmt::modulo(std::rand(), value); }

int gmt::rand_interval_centered(int value) { return gmt::modulo(std::rand(), value) - static_cast<int>(value / 2.0f); }

/* Return the cyclic interval in indexes */
/*
    ex: [1, 2, 3, 4, 5]
    [1, 3] => [1, 2, 3]
    [3, 1] => [3, 4, 5, 1]
*/
std::vector<int> gmt::cyclic_indexes(int min, int max, int size) {
    std::vector<int> indexes = {};
    if (min < max) {
        for (int i = min; i < max + 1; i++) { indexes.push_back(i); }
    } else {
        for (int i = min; i < size; i++) { indexes.push_back(i); }
        for (int i = 0; i < max + 1; i++) { indexes.push_back(i); }
    }
    return indexes;
}

std::vector<int> gmt::interpolate_array(const std::vector<int>& array_a, const std::vector<int>& array_b, const float& rate) {
    int size = std::min(array_a.size(), array_b.size());
    std::vector<int> array = {0, 0, 0, 0};
    for (int i = 0; i < size; i++) { array[i] = array_a[i] + (array_b[i] - array_a[i]) * rate; }
    return array;
}