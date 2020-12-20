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
	float size;

public:
	Circle(float x = 0.0f, float y = 0.0f, float size = 1.0f, float mass = 1.0f, float damping = 1.0f, float speed_x = 0.0f, float speed_y = 0.0f, float rotation = 0.0f, float motor = 0.0f, bool fixed = false, bool tied = false, bool etherial = false, sf::Color color = sf::Color::White);
	virtual ~Circle();

	int get_class() const;

	void Step();
	void Stop();
	void Move(float x, float y, bool relative = true);
	void Move(sf::Vector2f move, bool relative = true);
	bool inBounds(float x1, float x2, float y1, float y2);
	bool Pointed(float x, float y);

	void Collision(std::shared_ptr<Corpse> a);

	float get_size() const;
	ftn::Rectangle get_corpse_bounds() const;
};

}

#endif