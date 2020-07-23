#include "../include/Functional.hpp"

/* Dot product: [Ax Ay].[Bx By] = (Ax)(Bx)+(Ay)(By) = Amag*Bmag*cos(theta) */
float vtr::Dot(const sf::Vector2f &vect_a, const sf::Vector2f &vect_b) { return vect_a.x*vect_b.x + vect_a.y*vect_b.y; }
float vtr::Dot(float x1, float y1, float x2, float y2) { return x1*x2 + y1*y2; }

/* Vector length: c^2=a^2+b^2 <=> c = sqrt(a*a+b*b) */
float vtr::Length(const sf::Vector2f &vect) { return std::sqrt(Dot(vect, vect)); }
float vtr::Length(float x, float y) { return std::sqrt(Dot(x, y, x, y)); }
float vtr::Length(float x1, float y1, float x2, float y2) { return std::sqrt(Dot(x1-x2, y1-y2, x1-x2, y1-y2)); }

/* Convert deg rad: (PI/2) * (180/PI) = 90 */
float vtr::degree_to_radian(float degree) { return (degree / 180) * PI; }
float vtr::radian_to_degree(float radian) { return (radian / PI) * 180; }

/* Normalize vect = [Ax Ay].(1/size) = [Ax/size Ay/size] */
void vtr::Normalize(sf::Vector2f &vect) { 
	float length = Length(vect);
	if (length != 0 ) { vect /= length; } 
}

/* Rotate vector: [Ax Ay].scale = [Ax*scale Ay*scale] */
void vtr::Rotate(sf::Vector2f &vect, int angle) { 
	float theta = degree_to_radian(angle);
	float cos_theta = std::cos(theta);
	float sin_theta = std::sin(theta);
	vect = sf::Vector2f(vect.x * cos_theta - vect.y * sin_theta, vect.x * sin_theta + vect.y * cos_theta); 
}
/* Scale vector: [Ax Ay].scale = [Ax*scale Ay*scale] */
void vtr::Scale(sf::Vector2f &vect, float scale) { vect=sf::Vector2f(vect.x * scale, vect.y * scale); }

/* Return the float with a designed number of digits behind the comma */
float vtr::digits_comma(float number, int digits) { 
	int d_limit = std::pow(10, digits);
	return round(number * d_limit)/d_limit; 
}

/* Angle from a line: atan2(y, x) = Arg(x+iy) rad <=> atan2(y, x)*(180/3.1415)= Arg(x+iy) deg */
float vtr::bearing(float x1, float y1, float x2, float y2) { return atan2(y1 - y2, x1 - x2) * (180 / PI); }

bool vtr::rect_in_bounds(const vtr::Rectangle &object, const vtr::Rectangle &limits) {
	// One point in bounds
	// p1 (x,y)
	if (object.pos.x > limits.pos.x && object.pos.x < limits.pos.x + limits.size.x && object.pos.y > limits.pos.y && object.pos.y < limits.pos.y + limits.size.y) { return true; }
	// p2 (x+w,y)
	if (object.pos.x + object.size.x > limits.pos.x && object.pos.x + object.size.x < limits.pos.x + limits.size.x && object.pos.y > limits.pos.y && object.pos.y < limits.pos.y + limits.size.y) { return true; }
	// p3 (x,y+h)
	if (object.pos.x > limits.pos.x && object.pos.x < limits.pos.x + limits.size.x && object.pos.y + object.size.y > limits.pos.y && object.pos.y + object.size.y <  limits.pos.y + limits.size.y) { return true; }
	// p4 (x+w,y+h)
	if (object.pos.x + object.size.x > limits.pos.x && object.pos.x + object.size.x < limits.pos.x + limits.size.x  && object.pos.y + object.size.y > limits.pos.y && object.pos.y + object.size.y <  limits.pos.y + limits.size.y) { return true; }

	return false;
}

bool vtr::rect_out_bounds(const vtr::Rectangle &object, const vtr::Rectangle &limits) {
	// One face out bounds
	// f1
	if (object.pos.x < limits.pos.x) { return true; }
	// f2
	if (object.pos.x + object.size.x > limits.pos.x + limits.size.x) { return true; }
	// f3
	if (object.pos.y < limits.pos.y) { return true; }
	// f4 
	if (object.pos.y + object.size.y > limits.pos.y + limits.size.y) { return true; }

	return false;
}

