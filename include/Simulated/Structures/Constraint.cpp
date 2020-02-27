#include <string>
#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>

using namespace std;
using namespace sf;

#define PI 3.14159265

class Constraint : public Simulated
{
private:
	Body* bodyA; 
	Body* bodyB;

	float stiffness;
	float length;
	Color color;

	void Initialize(Body* bodyA, Body* bodyB, float stiffness, float length = 0.0f, Color color=Color::White)
	{
		this->bodyA = bodyA;
		this->bodyB = bodyB;
		this->stiffness = stiffness;
		this->color = color;

		if (length == 0.0f)
		{
			float distance = bodyA->Distance(bodyB);

			if (bodyA->Equals(bodyB)) { distance = 10.0f; }

			this->length = distance;
		}
		else
		{
			this->length = length;
		}
		
	}

public:
	Constraint(Body* bodyA, Body* bodyB)
	{
		Initialize(bodyA, bodyB, 0.3f);
	}

	Constraint(Body* bodyA, Body* bodyB, float stiffness)
	{
		Initialize(bodyA, bodyB, stiffness);
	}

	Constraint(Body* bodyA, Body* bodyB, float stiffness, Color color)
	{
		Initialize(bodyA, bodyB, stiffness, 0.0f, color);
	}

	Constraint(Body* bodyA, Body* bodyB, float stiffness, float length)
	{
		Initialize(bodyA, bodyB, stiffness, length);
	}

	void Move()
	{
		Body* bodyA = this->bodyA;
		Body* bodyB = this->bodyB;

		float distance = bodyA->Distance(bodyB);
		float distanceX = bodyA->DistanceX(bodyB);
		float distanceY = bodyA->DistanceY(bodyB);

		float difference = (this->length - distance) / (distance * this->stiffness);

		float offsetX = distanceX * difference * 0.5f;
		float offsetY = distanceY * difference * 0.5f;

		float mass = bodyA->getMass() + bodyB->getMass();
		float massB = bodyB->getMass() / mass;
		float massA = bodyA->getMass() / mass;

		if (bodyA->getFixed() || bodyB->getFixed())
		{
			if (bodyB->getFixed())
			{
				bodyA->setX( bodyA->getX() - offsetX );
				bodyA->setY( bodyA->getY() - offsetY );
			}

			if (bodyA->getFixed())
			{
				bodyB->setX( bodyB->getX() - offsetX );
				bodyB->setY( bodyB->getY() - offsetY );
			}
		}
		else
		{
			bodyA->setX( bodyA->getX() - offsetX * massB );
			bodyA->setY( bodyA->getY() - offsetY * massB );

			bodyB->setX( bodyB->getX() + offsetX * massA );
			bodyB->setY( bodyB->getY() + offsetY * massA );
		}	
	}

	Body* getD1() { return this->bodyA; }
	void setD1(Body* bodyA) { this->bodyA = bodyA; }
	Body* getD2() { return this->bodyB; }
	void setD2(Body* bodyB) { this->bodyB = bodyB; }

};