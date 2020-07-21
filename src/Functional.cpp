#include "../include/Functional.hpp"

float vtr::Dot(const sf::Vector2f vect_a, const sf::Vector2f vect_b) { return vect_a.x*vect_b.x + vect_a.y*vect_b.y; }
float vtr::Dot(float x1, float y1, float x2, float y2) { return x1*x2 + y1*y2; }

float vtr::Length(const sf::Vector2f vect) { return std::sqrt(Dot(vect, vect)); }
float vtr::Length(float x, float y) { return std::sqrt(Dot(x, y, x, y)); }
float vtr::Length(float x1, float y1, float x2, float y2) { return std::sqrt(Dot(x1-x2, y1-y2, x1-x2, y1-y2)); }

sf::Vector2f vtr::Normalize(sf::Vector2f vect) { 
	float length = Length(vect);
	if (length != 0 ) { vect /= length; } 
	return vect;
}

float vtr::degree_to_radian(float degree) { return (degree / 180) * PI; }
float vtr::radian_to_degree(float radian) { return (radian / PI) * 180; }

sf::Vector2f vtr::Rotate(const sf::Vector2f vect, int angle) { 
	float theta = degree_to_radian(angle);
	float cos_theta = std::cos(theta);
	float sin_theta = std::sin(theta);
	return sf::Vector2f(vect.x * cos_theta - vect.y * sin_theta, vect.x * sin_theta + vect.y * cos_theta); 
}

sf::Vector2f vtr::Scale(const sf::Vector2f vect, float scale) { return sf::Vector2f(vect.x * scale, vect.y * scale); }


float vtr::digits_comma(float number, int digits) { 
	int d_limit = std::pow(10, digits);
	return round(number * d_limit)/d_limit; 
}

float vtr::bearing(float x1, float y1, float x2, float y2) { return atan2(y1 - y2, x1 - x2) * (180 / PI); }


