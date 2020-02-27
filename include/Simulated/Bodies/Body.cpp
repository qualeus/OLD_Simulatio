#include <string>
#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>

#include "../Simulated.cpp"

#define PI 3.14159265

using namespace std;
using namespace sf;

class Body : public Simulated
{
protected:
	float x;
	float y;
	float x_speed;
	float y_speed;

	float mass;
	float size;
	float friction;


	Color color;
	string name;
public:

	Body(float x = 0.0f, float y = 0.0f, float mass = 1.00f, float size = 5.00f, float x_speed = 0.0f, float y_speed = 0.0f, float friction = 0.01f, Color color = Color::White, string name = "default",bool fixed = false, bool etherial = false):Simulated(fixed, etherial)
	{
		this->x = x;
		this->y = y;
		this->x_speed = x_speed;
		this->y_speed = y_speed;

		this->mass = mass;
		this->size = size;
		this->friction = friction;

		this->color = color;
		this->name = name;
	}

	virtual ~Body() { };

	virtual int getClass() override { return id_Body; }

	void CollisionCircle(Body* body){ };
	void CollisionPolygon(Body* body){ };
	void Gravity(Body* body){ };

	virtual void Step()
	{ 
		if (this->fixed)
		{
			this->x_speed = 0;
			this->y_speed = 0;
		}
		else
		{
			float acceleration = abs(1 - this->friction);

			if ( abs(this->x_speed) <= 0.01 )
			{
				this->x_speed = 0;
			}
			else
			{
				this->x_speed = this->x_speed * acceleration;
				this->x = this->x + this->x_speed;
			}

			if ( abs(this->y_speed) <= 0.01 )
			{
				this->y_speed = 0;
			}
			else
			{
				this->y_speed = this->y_speed * acceleration;
				this->y = this->y + this->y_speed;
			}
		}
	};

	bool Pointed(float x, float y){ return false; };

	bool Pairable(Simulated* corpse){ return this->getClass() == corpse->getClass() && !this->Equals(corpse); }

	void Move(float x, float y) { this->x = this->x + x; this->y = this->y + y; }
	void Launch(float x_speed, float y_speed) { this->x_speed = this->x_speed + x_speed; this->y_speed = this->y_speed + y_speed; }


	float Distance(Simulated* corpse) {
		switch (corpse->getClass())
		{
			case 1:
			{
				Body* body = dynamic_cast<Body*>(corpse);
				return sqrt( pow(this->x - body->x, 2) + pow(this->y - body->y, 2)); 
			} break;

			default:
				return 0;
		}
	}

	float DistanceX(Simulated* corpse) { 
		switch (corpse->getClass())
		{
			case 1:
			{
				Body* body = dynamic_cast<Body*>(corpse);
				return this->x - body->x;
			} break;

			default:
				return 0;
		}
	}

	float DistanceY(Simulated* corpse) { 
		switch (corpse->getClass())
		{
			case 1:
			{
				Body* body = dynamic_cast<Body*>(corpse);
				return this->y - body->y; 
			} break;

			default:
				return 0;
		}
	}

	float Distance(float x, float y) { 
		return sqrt( pow(this->x - x, 2) + pow(this->y - y, 2)); 
	}

	
	bool Collidable(Simulated* corpse) { return !this->getEtherial() && !corpse->getEtherial(); }

	bool Collide(Simulated* corpse) { 
		switch (corpse->getClass())
		{
			case 1:
			{
				Body* body = dynamic_cast<Body*>(corpse);
				return this->Distance(body) < (this->getSize() + body->getSize()) && this->Collidable(body);
			} break;

			default:
				return 0;
		}
	}

	float getX() { return this->x; }
	float getY() { return this->y; }

	void setX(float x) { this->x = x; }
	void setY(float y) { this->y = y; }

	float getX_speed() { return this->x_speed; }
	float getY_speed() { return this->y_speed; }

	void setX_speed(float x) { this->x_speed = x_speed; }
	void setY_speed(float y) { this->y_speed = y_speed; }

	float getMass() { return this->mass; }
	void setMass(float mass) { this->mass = mass; }

	float getSize() { return this->size; }
	void setSize(float size) { this->size = size;}

	float getFriction() { return this->friction; }
	void setFriction(float friction) { this->friction = friction; }

	Color getColor() { return this->color; }
	void setColor(Color color) { this->color = color; }	
};