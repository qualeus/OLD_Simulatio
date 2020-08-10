#ifndef Corpse_HPP
#define Corpse_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <iostream>
#include <cmath>

#include "../include/Functional.hpp"

namespace phy {

const static int ID_CORPSE = 1;

class Corpse
{
protected:
	int id;
	bool fixed;
	bool etherial;
	bool removed;
	float friction;
	float mass;
	float damping;
	sf::Vector2f current_pos;
	sf::Vector2f last_pos;
	sf::Color color;

public:
	explicit Corpse(float x, float y, float mass, float damping, bool fixed, bool etherial, sf::Color color); // System Constructor
	virtual ~Corpse(); // System Destructor

	int get_id() const;
	virtual int get_class() const;

	bool get_fixed() const;
	void set_fixed(bool fixed);

	bool get_etherial() const;
	void set_etherial(bool etherial);

	bool get_removed() const;
	void Remove();

	virtual void Step();
	virtual void Stop();
	virtual void Move(float x, float y, bool relative = true);
	virtual void Move(sf::Vector2f move, bool relative = true);
	virtual bool inBounds(float x1, float x2, float y1, float y2) = 0;
	virtual bool Pointed(float x, float y) = 0;

	virtual void Collision(std::shared_ptr<Corpse> a);
	sf::Vector2f get_diff_pos() const;

	sf::Color get_color() const;
	void set_color(sf::Color color);

	sf::Vector2f get_pos() const;
	float get_pos_x() const;
	float get_pos_y() const;

	void set_pos(sf::Vector2f pos);
	void set_pos_x(float pos_x);
	void set_pos_y(float pos_y);

	sf::Vector2f get_last_pos() const;
	float get_last_pos_x() const;
	float get_last_pos_y() const;

	void set_last_pos(sf::Vector2f pos);
	void set_last_pos_x(float pos_x);
	void set_last_pos_y(float pos_y);

	void set_damping(float damping);
	float get_bounce() const;
	float get_mass() const;
	virtual ftn::Rectangle get_corpse_bounds() const = 0;

	bool Equals(const Corpse* other);
	inline bool operator==(const Corpse* other);
};

}

#endif