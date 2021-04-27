#ifndef Text_HPP
#define Text_HPP

#include "Maths.hpp"
#include "Vector.hpp"

namespace gmt {

template <typename T>
class Text {
   public:
    std::string text;
    Vector<T> pos;
    T size;
    bool fixed;
    sf::Color color;

    Text<T> operator<<(const Text<T>& other) const;

    Text();
    ~Text();

    Text(const std::string& text, const T& x, const T& y, const T& size, const bool& fixed, const sf::Color& color);
    Text(const std::string& text, const Vector<T>& pos, const T& size, const bool& fixed, const sf::Color& color);
};
}  // namespace gmt
#endif