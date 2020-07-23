#ifndef Functional_HPP
#define Functional_HPP

#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <memory>
#include <sstream>
#include <iostream>

#define PI 3.141592654

namespace vtr {

struct Rectangle {
	sf::Vector2f pos;
	sf::Vector2f size;
};

struct Text {
	std::string str = "";
	float x = 0.0f;
	float y = 0.0f;
	float size = 18;
	bool fixed = false;
	sf::Color color = sf::Color::White;
};

float Dot(const sf::Vector2f &vect_a, const sf::Vector2f &vect_b);
float Dot(float x1, float y1, float x2, float y2);

float Length(const sf::Vector2f &vect);
float Length(float x, float y);
float Length(float x1, float y1, float x2, float y2);

float degree_to_radian(float degree);
float radian_to_degree(float radian);

void Normalize(sf::Vector2f &vect);
void Rotate(sf::Vector2f &vect, int angle);
void Scale(sf::Vector2f &vect, float scale);

float digits_comma(float number, int digits);

float bearing(float x1, float y1, float x2, float y2);

bool rect_in_bounds(const vtr::Rectangle &object, const vtr::Rectangle &limits);
bool rect_out_bounds(const vtr::Rectangle &object, const vtr::Rectangle &limits);

template<class C>
std::shared_ptr<C> remove(int i, std::vector<std::shared_ptr<C>> &vect) {
	std::shared_ptr<C> corpse = vect.at(i);
	vect.erase(vect.begin()+i);
	return corpse;
}

template <typename T>
std::string to_string(T value) {
	std::ostringstream oss;
	oss << value;
	return oss.str();
} 

}
#endif