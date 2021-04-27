#include "../../include/Geometry/Text.hpp"

namespace gmt {

template <typename T>
Text<T> Text<T>::operator<<(const Text<T>& other) const {
    return Text<T>(this->text + other.text, this->pos, this->size, this->fixed, this->color);
}
template Text<int> Text<int>::operator<<(const Text<int>& other) const;
template Text<float> Text<float>::operator<<(const Text<float>& other) const;
template Text<double> Text<double>::operator<<(const Text<double>& other) const;

/* Default Constructor */
template <typename T>
Text<T>::Text() {
    this->text = "";
    this->pos = this->pos.Clone();
    this->size = T(17);
    this->fixed = false;
    this->color = sf::Color();
}
template Text<int>::Text();
template Text<float>::Text();
template Text<double>::Text();

/* Default Destructor */
template <typename T>
Text<T>::~Text() {}
template Text<int>::~Text();
template Text<float>::~Text();
template Text<double>::~Text();

/* Builder Constructor */
template <typename T>
Text<T>::Text(const std::string& text, const T& x, const T& y, const T& size, const bool& fixed, const sf::Color& color) {
    this->text = text;
    this->pos = Vector<T>(x, y);
    this->size = size;
    this->fixed = fixed;
    this->color = color;
}
template Text<int>::Text(const std::string& text, const int& x, const int& y, const int& size, const bool& fixed, const sf::Color& color);
template Text<float>::Text(const std::string& text, const float& x, const float& y, const float& size, const bool& fixed, const sf::Color& color);
template Text<double>::Text(const std::string& text, const double& x, const double& y, const double& size, const bool& fixed, const sf::Color& color);

/* Builder Constructor */
template <typename T>
Text<T>::Text(const std::string& text, const Vector<T>& pos, const T& size, const bool& fixed, const sf::Color& color) {
    this->text = text;
    this->pos = pos;
    this->size = size;
    this->fixed = fixed;
    this->color = color;
}
template Text<int>::Text(const std::string& text, const Vector<int>& pos, const int& size, const bool& fixed, const sf::Color& color);
template Text<float>::Text(const std::string& text, const Vector<float>& pos, const float& size, const bool& fixed, const sf::Color& color);
template Text<double>::Text(const std::string& text, const Vector<double>& pos, const double& size, const bool& fixed, const sf::Color& color);
}  // namespace gmt