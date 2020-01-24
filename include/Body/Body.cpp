#include <string>
#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>

#define PI 3.14159265

using namespace std;
using namespace sf;

class Body
{
protected:
	float x, y;
	string name;


	float mass;

	bool fixed;
	bool etherial;

	Body(float x, float y, float mass = 1.0f, string name = "default", bool fixed = false, bool etherial = false)
	{
		this->x = x;
		this->y = y;
		this->mass = mass;
		this->name = name;
		this->fixed = fixed;
		this->etherial = etherial;
	}
public:
	string getName() { return this->name; }

	float getX() { return this->x; }
	void setX(float x) { this->x = x; }
	float getY() { return this->y; }
	void setY(float y) { this->y = y; }

	float getMass() { return this->mass; }
	void setMass(float mass) { this->mass = mass; }

	bool isFixed(){ return this->fixed; }
	void setFixed(bool fixed){ this->fixed = fixed; }
	bool isEtherial() { return this->etherial; }
	void setEtherial(bool etherial){ this->etherial = etherial; }

	bool Equals(Body b) { return (this->mass == b.mass && this->x == b.x && this->y == b.y && this->fixed == b.fixed && this->etherial == b.etherial); }
};