#include <string>
#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>
using namespace std;
using namespace sf;

#define PI 3.14159265
class Constraint
{
private:
	Dot* d1; 
	Dot* d2;
	float stiffness;
	float length;
	Color color;

	void Initialize(Dot* d1, Dot* d2, float stiffness, float length = 0.0f, Color color=Color::White)
	{
		this->d1 = d1;
		this->d2 = d2;
		this->stiffness = stiffness;
		this->color = color;

		Dot& rd1 = *d1;
		Dot& rd2 = *d2;
		
		Dot& p1 = *this->d1;
		Dot& p2 = *this->d2;

		float dx = p2.getX() - p1.getX();
		float dy = p2.getY() - p1.getY();

		if (length == 0.0f)
		{
			this->length = sqrt(pow(dx, 2) + pow(dy, 2));
		}
		else
		{
			this->length = length;
		}
		
	}
public:
	Constraint(Dot* d1, Dot* d2)
	{
		Initialize(d1, d2, 0.3f);
	}

	Constraint(Dot* d1, Dot* d2, float stiffness)
	{
		Initialize(d1, d2, stiffness);
	}

	Constraint(Dot* d1, Dot* d2, float stiffness, Color color)
	{
		Initialize(d1, d2, stiffness, 0.0f, color);
	}

	Constraint(Dot* d1, Dot* d2, float stiffness, float length)
	{
		Initialize(d1, d2, stiffness, length);
	}

	void Move()
	{
		Dot& p1 = *this->d1;
		Dot& p2 = *this->d2;

		float dx = p2.getX() - p1.getX();
		float dy = p2.getY() - p1.getY();

		float dist = sqrt(pow(dx, 2) + pow(dy, 2));
		float diff = (this->length - dist) / (dist * this->stiffness);

		float offsetX = dx * diff * 0.5f;
		float offsetY = dy * diff * 0.5f;

		float m2 = p1.getMass() / (p1.getMass() + p2.getMass());
		float m1 = p2.getMass() / (p1.getMass() + p2.getMass());

		if (!p1.isFixed() && !p2.isFixed())
		{
			p1.setX(p1.getX() - offsetX * m1);
			p1.setY(p1.getY() - offsetY * m1);
			p2.setX(p2.getX() + offsetX * m2);
			p2.setY(p2.getY() + offsetY * m2);
		}
		else
		{
			if (!p1.isFixed())
			{
				p1.setX(p1.getX() - offsetX * (m1 + m2));
				p1.setY(p1.getY() - offsetY * (m1 + m2));
			}

			if (!p2.isFixed())
			{
				p2.setX(p2.getX() + offsetX * (m1 + m2));
				p2.setY(p2.getY() + offsetY * (m1 + m2));
			}
		}	
	}
	

	Dot* getD1() { return this->d1; }
	void setD1(Dot* d1) { this->d1 = d1; }
	Dot* getD2() { return this->d2; }
	void setD2(Dot* d2) { this->d2 = d2; }

};