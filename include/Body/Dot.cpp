#include "Body.cpp"

using namespace std;
using namespace sf;

class Dot : public Body
{
private:
	float spdX, spdY;
	float friction;

	float size;
	Color color;

public:

	Dot()
	:Body(0.0f, 0.0f, 1.0f)
	{
		this->spdX = 0.0f;
		this->spdY = 0.0f;
		this->size = 1.0f;
		this->friction = 0.0f;
		this->color = Color::White;
	}

	Dot(float x, float y,string name, float mass, float size)
	:Body(x, y, mass, name)
	{
		this->spdX = 0.0f;
		this->spdY = 0.0f;
		this->size = size;
		this->friction = 0.0f;
		this->color = Color::White;
	}

	Dot(float x, float y, string name, float mass, float size, float spdX, float spdY)
	:Body(x, y, mass, name)
	{
		this->spdX = spdX;
		this->spdY = spdY;
		this->size = size;
		this->friction = 0.0f;
		this->color = Color::White;
	}

	Dot(float x, float y, string name, float mass, float size, float spdX, float spdY, float friction)
	:Body(x, y, mass, name)
	{
		this->spdX = spdX;
		this->spdY = spdY;
		this->size = size;
		this->friction = friction;
		this->color = Color::White;
	}

	Dot(float x, float y, string name, float mass, float size, float spdX, float spdY, float friction, bool fixed, bool etherial, Color color=Color::White)
	:Body(x, y, mass, name, fixed, etherial)
	{
		this->spdX = spdX;
		this->spdY = spdY;
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

	

	float getspdX() { return this->spdX; }
	void setspdX(float spdX) { this->spdX = spdX; }
	float getspdY() { return this->spdY; }
	void setspdY(float spdY) { this->spdY = spdY; }

	float getSize() { return this->size; }

	Color getColor() { return this->color; }
	void setColor(Color color) { this->color = color; }

	
};