#include "../../include/Renderer/Converter.hpp"

/* Geometry vector to SFML vector */
template <typename T>
sf::Vector2<T> convertSF(const gmt::Vector<T>& vect) {
    return sf::Vector2<T>(vect.x, vect.y);
}
template sf::Vector2<int> convertSF(const gmt::Vector<int>& v);
template sf::Vector2<float> convertSF(const gmt::Vector<float>& v);
template sf::Vector2<double> convertSF(const gmt::Vector<double>& v);

/* SFML vector to Geometry vector */
template <typename T>
gmt::Vector<T> convertSF(const sf::Vector2<T>& vect) {
    return gmt::Vector<T>(vect.x, vect.y);
}
template gmt::Vector<int> convertSF(const sf::Vector2<int>& v);
template gmt::Vector<float> convertSF(const sf::Vector2<float>& v);
template gmt::Vector<double> convertSF(const sf::Vector2<double>& v);
