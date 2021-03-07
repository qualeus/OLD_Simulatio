#ifndef Vector_HPP
#define Vector_HPP

#include <SFML/Graphics.hpp>
#include <cmath>

#include "../../include/Geometry/Geometry.hpp"
namespace gmt {

template <typename T>
class Vector {
   public:
    T x;
    T y;

    Vector<T> operator+(const Vector<T>& other) const;
    Vector<T> operator-(const Vector<T>& other) const;
    Vector<T> operator*(const T& other) const;
    Vector<T> operator/(const T& other) const;

    Vector(const T& x, const T& y);
    Vector();
    T Magnitude() const;
    T MagnitudeSquared() const;

    Vector<T> Clone() const;
    sf::Vector2<T> CloneSF() const;

    Vector<T> Normalize() const;
    Vector<T> Scale(const T& scale) const;
    Vector<T> Pow(const int& power) const;
    Vector<T> Rotate(const float& angle) const;
    Vector<T> SetAngle(const float& angle) const;

    static T Distance(const Vector<T>& v1, const Vector<T>& v2);
    static T DistanceSquared(const Vector<T>& v1, const Vector<T>& v2);
    static T Dot(const Vector<T>& v1, const Vector<T>& v2);
    static T Cross(const Vector<T>& v1, const Vector<T>& v2);
};
}  // namespace gmt

#endif