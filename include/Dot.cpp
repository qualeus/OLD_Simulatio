#include "Body.cpp"

using namespace std;
using namespace sf;

class Dot : public Body
{
private:
	float spdX, spdY;
	int mass;
	float friction;

	float size;
	Color color;

public:

	Dot()
	:Body(0.0f, 0.0f)
	{
		this->spdX = 0.0f;
		this->spdY = 0.0f;
		this->mass = 0;
		this->size = 1.0f;
		this->friction = 0.0f;
		this->color = Color::White;
	}

	Dot(float x, float y,string name, int mass, float size)
	:Body(x, y, name)
	{
		this->spdX = 0.0f;
		this->spdY = 0.0f;
		this->mass = mass;
		this->size = size;
		this->friction = 0.0f;
		this->color = Color::White;
	}

	Dot(float x, float y, string name, int mass, float size, float spdX, float spdY)
	:Body(x, y, name)
	{
		this->spdX = spdX;
		this->spdY = spdY;
		this->mass = mass;
		this->size = size;
		this->friction = 0.0f;
		this->color = Color::White;
	}

	Dot(float x, float y, string name, int mass, float size, float spdX, float spdY, float friction)
	:Body(x, y, name)
	{
		this->spdX = spdX;
		this->spdY = spdY;
		this->mass = mass;
		this->size = size;
		this->friction = friction;
		this->color = Color::White;
	}

	Dot(float x, float y, string name, int mass, float size, float spdX, float spdY, float friction, bool fixed, bool etherial, Color color=Color::White)
	:Body(x, y, name, fixed, etherial)
	{
		this->spdX = spdX;
		this->spdY = spdY;
		this->mass = mass;
		this->size = size;
		this->friction = friction;
		this->color = color;
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
	void setColor(Color color) { this->color = color; }

	bool isFixed(){ return this->fixed; }
	void setFixed(bool fixed){ this->fixed = fixed; }
	bool isEtherial() { return this->etherial; }
	void setEtherial(bool etherial){ this->etherial = etherial; }

	bool Equals(Dot b) { return (this->mass == b.mass && this->x == b.x && this->y == b.y && this->spdX == b.spdX && this->spdY == b.spdY); }
};