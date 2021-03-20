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

/* Retunr the positive modulo for a int */
unsigned gmt::modulo(int value, unsigned m) {
    int mod = value % (int)m;
    if (mod < 0) { mod += m; }
    return mod;
}
