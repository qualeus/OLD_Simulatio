#ifndef Circle_HPP
#define Circle_HPP

#include <SFML/Graphics.hpp>
#include <vector>

#include "../include/Corpse.hpp"
#include "../include/Polygon.hpp"

namespace phy {

const static int ID_CIRCLE = 2;

class Circle : public Corpse
{
private:
	sf::Vector2f last_pos;
	float size;
	sf::Color color;

public:
	Circle(float x = 0.0f, float y = 0.0f, float size = 1.0f, float mass = 1.0f, float damping = 1.0f, float speed_x = 0.0f, float speed_y = 0.0f, bool fixed = false, bool etherial = false, sf::Color color = sf::Color::White);
	virtual ~Circle();

	const int get_class();

	void Step();
	void Stop();
	void Move(float x, float y);
	void Move(sf::Vector2f move);
	bool inBounds(float x1, float x2, float y1, float y2);
	bool Pointed(float x, float y);

	void Collision(std::shared_ptr<Corpse> a);

	sf::Color get_color();
	void set_color(sf::Color color);

	sf::Vector2f get_last_pos();
	float get_last_pos_x();
	float get_last_pos_y();

	void set_last_pos(sf::Vector2f pos);
	void set_last_pos_x(float pos_x);
	void set_last_pos_y(float pos_y);

	float get_size();
	vtr::Rectangle get_corpse_bounds();
};

}

#endif