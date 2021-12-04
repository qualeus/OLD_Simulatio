#ifndef Converter_HPP
#define Converter_HPP

#include <SFML/Graphics.hpp>

#include "../Geometry/Geometry.hpp"

template <typename T>
sf::Vector2<T> convertSF(const gmt::Vector<T>& vect);

template <typename T>
gmt::Vector<T> convertSF(const sf::Vector2<T>& vect);

#endif