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
	float mass;
	float damping;
	sf::Vector2f current_pos;

public:
	explicit Corpse(float x, float y, float mass, float damping, bool fixed, bool etherial); // System Constructor
	virtual ~Corpse(); // System Destructor

	int get_id() const;
	virtual int get_class() const;

	bool get_fixed() const;
	void set_fixed(bool fixed);

	bool get_etherial() const;
	void set_etherial(bool etherial);

	virtual void Step();
	virtual void Stop();
	virtual void Move(float x, float y);
	virtual void Move(sf::Vector2f move);
	virtual bool inBounds(float x1, float x2, float y1, float y2) = 0;
	virtual bool Pointed(float x, float y) = 0;

	virtual void Collision(std::shared_ptr<Corpse> a);

	sf::Vector2f get_pos();
	float get_pos_x();
	float get_pos_y();

	void set_pos(sf::Vector2f pos);
	void set_pos_x(float pos_x);
	void set_pos_y(float pos_y);

	void set_damping(float damping);
	float get_bounce();
	float get_mass();
	virtual float get_size() = 0;
	virtual vtr::Rectangle get_corpse_bounds() = 0;

	bool Equals(const Corpse* other);
	inline bool operator==(const Corpse* other);
};

}

#endif