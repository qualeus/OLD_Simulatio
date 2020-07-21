#ifndef Functional_HPP
#define Functional_HPP

#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <memory>

#define PI 3.141592654

namespace vtr {

struct Rectangle {
	sf::Vector2f pos;
	sf::Vector2f size;
};

float Dot(const sf::Vector2f vect_a, const sf::Vector2f vect_b);
float Dot(float x1, float y1, float x2, float y2);

float Length(const sf::Vector2f vect);
float Length(float x, float y);
float Length(float x1, float y1, float x2, float y2);

sf::Vector2f Normalize(sf::Vector2f vect);

float degree_to_radian(float degree);
float radian_to_degree(float radian);

sf::Vector2f Rotate(const sf::Vector2f vect, int angle);
sf::Vector2f Scale(const sf::Vector2f vect, float scale);

float digits_comma(float number, int digits);

// return an angle from a line
float bearing(float x1, float y1, float x2, float y2);

template<class C>
std::shared_ptr<C> remove(int i, std::vector<std::shared_ptr<C>> &vect) {
	std::shared_ptr<C> corpse = vect.at(i);
	vect.erase (vect.begin()+i);
	return corpse;
}

}
#endif