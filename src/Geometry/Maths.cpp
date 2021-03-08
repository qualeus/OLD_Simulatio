#include "../../include/Geometry/Maths.hpp"

bool gmt::Equals(float a, float b, float sigma) { return (std::abs(b - a) < std::abs(sigma)); }

/* Convert deg rad: (PI/2) * (180/PI) = 90 */
float gmt::degree_to_radian(float degree) { return (degree / 180) * PI; }
float gmt::radian_to_degree(float radian) { return (radian / PI) * 180; }

/* Return the float with a designed number of digits behind the comma */
float gmt::digits_comma(float number, int digits) {
    int d_limit = std::pow(10, digits);
    return round(number * d_limit) / d_limit;
}