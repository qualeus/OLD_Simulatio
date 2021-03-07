#include "../../include/Geometry/Vector.hpp"

namespace gmt {

/* Vectors Operators: [Ax Ay]+[Bx By] = [Ax+Bx Ay+By] */
template <typename T>
Vector<T> Vector<T>::operator+(const Vector<T>& other) const {
    return Vector<T>(this->x + other.x, this->y + other.y);
}
template Vector<int> Vector<int>::operator+(const Vector<int>& other) const;
template Vector<float> Vector<float>::operator+(const Vector<float>& other) const;
template Vector<double> Vector<double>::operator+(const Vector<double>& other) const;

/* Vectors Operators: [Ax Ay]-[Bx By] = [Ax-Bx Ay-By] */
template <typename T>
Vector<T> Vector<T>::operator-(const Vector<T>& other) const {
    return Vector<T>(this->x - other.x, this->y - other.y);
}
template Vector<int> Vector<int>::operator-(const Vector<int>& other) const;
template Vector<float> Vector<float>::operator-(const Vector<float>& other) const;
template Vector<double> Vector<double>::operator-(const Vector<double>& other) const;

/* Scalars Operators [Ax Ay]*X = [Ax*X Ay*X] */
template <typename T>
Vector<T> Vector<T>::operator*(const T& other) const {
    return Vector<T>(this->x * other, this->y * other);
}
template Vector<int> Vector<int>::operator*(const int& other) const;
template Vector<float> Vector<float>::operator*(const float& other) const;
template Vector<double> Vector<double>::operator*(const double& other) const;

/* Scalars Operators [Ax Ay]/X = [Ax/X Ay/X] */
template <typename T>
Vector<T> Vector<T>::operator/(const T& other) const {
    return Vector<T>(this->x / other, this->y / other);
}
template Vector<int> Vector<int>::operator/(const int& other) const;
template Vector<float> Vector<float>::operator/(const float& other) const;
template Vector<double> Vector<double>::operator/(const double& other) const;

/* Default Constructor */
template <typename T>
Vector<T>::Vector() {
    this->x = T();
    this->y = T();
}
template Vector<int>::Vector();
template Vector<float>::Vector();
template Vector<double>::Vector();

/* Builder Constructor */
template <typename T>
Vector<T>::Vector(const T& x, const T& y) {
    this->x = x;
    this->y = y;
}
template Vector<int>::Vector(const int& x, const int& y);
template Vector<float>::Vector(const float& x, const float& y);
template Vector<double>::Vector(const double& x, const double& y);

/* Vector length: ||A|| = size = sqrt(Ax*Ax + Ay*Ay) */
template <typename T>
T Vector<T>::Magnitude() const {
    return std::sqrt(Vector<T>::Dot(*this, *this));
}
template int Vector<int>::Magnitude() const;
template float Vector<float>::Magnitude() const;
template double Vector<double>::Magnitude() const;

/* Vector length Squared: size*size = Ax*Ax + Ay*Ay */
template <typename T>
T Vector<T>::MagnitudeSquared() const {
    return Vector<T>::Dot(*this, *this);
}
template int Vector<int>::MagnitudeSquared() const;
template float Vector<float>::MagnitudeSquared() const;
template double Vector<double>::MagnitudeSquared() const;

/* Duplication Method */
template <typename T>
Vector<T> Vector<T>::Clone() const {
    return Vector<T>(this->x, this->y);
}
template Vector<int> Vector<int>::Clone() const;
template Vector<float> Vector<float>::Clone() const;
template Vector<double> Vector<double>::Clone() const;

/* Convert to SFML */
template <typename T>
sf::Vector2<T> Vector<T>::CloneSF() const {
    return sf::Vector2<T>(this->x, this->y);
}
template sf::Vector2<int> Vector<int>::CloneSF() const;
template sf::Vector2<float> Vector<float>::CloneSF() const;
template sf::Vector2<double> Vector<double>::CloneSF() const;

/* Normalize vector = [Ax Ay].(1/size) = [Ax/size Ay/size] */
template <typename T>
Vector<T> Vector<T>::Normalize() const {
    T magnitude = this->Magnitude();
    if (magnitude == T()) { return Vector<T>(); }  // Avoid dividing by 0
    return Vector<T>(this->x / magnitude, this->y / magnitude);
}
template Vector<int> Vector<int>::Normalize() const;
template Vector<float> Vector<float>::Normalize() const;
template Vector<double> Vector<double>::Normalize() const;

/* Scale vector: [Ax Ay].scale = [Ax*scale Ay*scale] */
template <typename T>
Vector<T> Vector<T>::Scale(const T& scale) const {
    return Vector<T>(this->x * scale, this->y * scale);
}
template Vector<int> Vector<int>::Scale(const int& scale) const;
template Vector<float> Vector<float>::Scale(const float& scale) const;
template Vector<double> Vector<double>::Scale(const double& scale) const;

/* Power vector: [Ax Ay]^pow = [Ax^pow Ay^pow] */
template <typename T>
Vector<T> Vector<T>::Pow(const int& power) const {
    Vector<T> temp = this->Clone();
    for (int i = 0; i < power - 1; i++) {
        temp.x = temp.x * temp.x;
        temp.y = temp.y * temp.y;
    }
    return temp;
}

/* Rotate vector: [Ax Ay] = [Ax*cos(theta)-Ay*sin(theta) Ax*sin(theta)+Ay*cos(theta)] */
template <typename T>
Vector<T> Vector<T>::Rotate(const float& angle) const {
    float theta = degree_to_radian(angle);
    float cos_theta = std::cos(theta);
    float sin_theta = std::sin(theta);
    return Vector<T>(this->x * cos_theta - this->y * sin_theta, this->x * sin_theta + this->y * cos_theta);
}
template Vector<int> Vector<int>::Rotate(const float& angle) const;
template Vector<float> Vector<float>::Rotate(const float& angle) const;
template Vector<double> Vector<double>::Rotate(const float& angle) const;

/* Set the angle: [Ax Ay] = [scale*cos(theta), scale*sin(theta)] */
template <typename T>
Vector<T> Vector<T>::SetAngle(const float& angle) const {
    float theta = degree_to_radian(angle);
    float cos_theta = std::cos(theta);
    float sin_theta = std::sin(theta);
    T magnitude = this->Magnitude();
    return Vector<T>(magnitude * cos_theta, magnitude * sin_theta);
}
template Vector<int> Vector<int>::SetAngle(const float& angle) const;
template Vector<float> Vector<float>::SetAngle(const float& angle) const;
template Vector<double> Vector<double>::SetAngle(const float& angle) const;

/* Dot product: [Ax Ay].[Bx By] = (Ax)(Bx)+(Ay)(By) = Amag*Bmag*cos(theta) */
template <typename T>
T Vector<T>::Dot(const Vector<T>& v1, const Vector<T>& v2) {
    return v1.x * v2.x + v1.y * v2.y;
}
template int Vector<int>::Dot(const Vector<int>& v1, const Vector<int>& v2);
template float Vector<float>::Dot(const Vector<float>& v1, const Vector<float>& v2);
template double Vector<double>::Dot(const Vector<double>& v1, const Vector<double>& v2);

/* Perp/Cross Dot product: [Ax Ay].[Bx By] = (Ax)(Bx)-(Ay)(By) = Amag*Bmag*sin(theta) */
template <typename T>
T Vector<T>::Cross(const Vector<T>& v1, const Vector<T>& v2) {
    return v1.x * v2.y - v1.y * v2.x;
}
template int Vector<int>::Cross(const Vector<int>& v1, const Vector<int>& v2);
template float Vector<float>::Cross(const Vector<float>& v1, const Vector<float>& v2);
template double Vector<double>::Cross(const Vector<double>& v1, const Vector<double>& v2);

/* Vector length: ||A|| = size = sqrt(Ax*Ax + Ay*Ay) */
template <typename T>
T Vector<T>::Distance(const Vector<T>& v1, const Vector<T>& v2) {
    return std::sqrt(Vector::Dot(v1 - v2, v1 - v2));
}
template int Vector<int>::Distance(const Vector<int>& v1, const Vector<int>& v2);
template float Vector<float>::Distance(const Vector<float>& v1, const Vector<float>& v2);
template double Vector<double>::Distance(const Vector<double>& v1, const Vector<double>& v2);

/* Vector length Squared: size*size = Ax*Ax + Ay*Ay */
template <typename T>
T Vector<T>::DistanceSquared(const Vector<T>& v1, const Vector<T>& v2) {
    return Vector::Dot(v1 - v2, v1 - v2);
}
template int Vector<int>::DistanceSquared(const Vector<int>& v1, const Vector<int>& v2);
template float Vector<float>::DistanceSquared(const Vector<float>& v1, const Vector<float>& v2);
template double Vector<double>::DistanceSquared(const Vector<double>& v1, const Vector<double>& v2);

}  // namespace gmt
