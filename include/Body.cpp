#include <string>
#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>

using namespace std;
using namespace sf;

#define PI 3.14159265
class Body
{
private:
	float x, y;
	string name;
	float spdX, spdY;
	int mass;
	float friction;

	float size;
	Color color;

	bool fixed;
	bool etherial;

	void Initialize(float x, float y, string name, float spdX, float spdY, int mass, float size, float friction, bool fixed=false, bool etherial=false, Color color=Color::White)
	{
		this->x = x;
		this->y = y;
		this->name = name;
		this->spdX = spdX;
		this->spdY = spdY;
		this->mass = mass;
		this->size = size;
		this->friction = friction;
		this->fixed = fixed;
		this->etherial = etherial;
		this->color = color;
	}

public:
	Body()
	{
		Initialize(0.0f, 0.0f, "Default", 0.0f, 0.0f, 0, 0.0f, 0.0f);
	}

	Body(float x, float y,string name, int mass, float size)
	{
		Initialize(x, y, name, 0.0f, 0.0f, mass, size, 0.0f);
	}

	Body(float x, float y, string name, int mass, float size, float spdX, float spdY)
	{
		Initialize(x, y, name, spdX, spdY, mass, size, 0.0f);
	}

	Body(float x, float y, string name, int mass, float size, float spdX, float spdY, float friction)
	{
		Initialize(x, y, name, spdX, spdY, mass, size, friction);
	}

	Body(float x, float y, string name, int mass, float size, float spdX, float spdY, float friction, bool fixed, bool etherial, Color color=Color::White)
	{
		Initialize(x, y, name, spdX, spdY, mass, size, friction, fixed, etherial, color);
	}

	void Move()
	{
		if (this->fixed)
		{
			this->spdX = 0;
			this->spdY = 0;
		}
		else
		{
			if (abs(this->spdX) < 0.01)
			{
				this->spdX = 0;
			}

			if (abs(this->spdY) < 0.01)
			{
				this->spdY = 0;
			}

			this->spdX = this->spdX * (1 - this->friction);
			this->spdY = this->spdY * (1 - this->friction);

			this->x = this->x + this->spdX;
			this->y = this->y + this->spdY;
		}
	}

	string getName() { return this->name; }

	float getX() { return this->x; }
	void setX(float x) { this->x = x; }
	float getY() { return this->y; }
	void setY(float y) { this->y = y; }

	float getspdX() { return this->spdX; }
	void setspdX(float spdX) { this->spdX = spdX; }
	float getspdY() { return this->spdY; }
	void setspdY(float spdY) { this->spdY = spdY; }

	float getSize() { return this->size; }
	float getMass() { return this->mass; }
	Color getColor() { return this->color; }

	bool isFixed(){ return this->fixed; }
	bool isEtherial() { return this->etherial; }

	bool Equals(Body b) { return (this->mass == b.mass && this->x == b.x && this->y == b.y && this->spdX == b.spdX && this->spdY == b.spdY); }
};