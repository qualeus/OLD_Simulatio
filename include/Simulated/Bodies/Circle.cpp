#include "Body.cpp"

using namespace std;
using namespace sf;

class Circle : public Body
{
private:

public:

	Circle(float x = 0.0f, float y = 0.0f, float mass = 1.0f, float size= 1.0f, float x_speed = 0.0f, float y_speed = 0.0f, float friction= 0.0f, Color color = Color::White, string name = "Default", bool fixed = false, bool etherial = false):Body(x, y, mass, size, x_speed, y_speed, friction, color, name, fixed, etherial)
	{ 

	}

	virtual ~Circle() { };

	virtual int getClass() override { return id_Circle; }

	void Step()
	{
		if(this->fixed)
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
	}

	bool Pointed(float x, float y) { return this->Distance(x, y) <= this->getSize(); }

	void Collision(Simulated* corpse)
	{
		switch (corpse->getClass())
		{
			case id_Body:
			{
				Body* body = dynamic_cast<Body*>(corpse);

			} break;

			case id_Circle:
			{
				Circle* circle = dynamic_cast<Circle*>(corpse);
				CollisionCircle(circle);

			} break;
		}
	}

	void CollisionCircle(Body* circle)
	{
		float distance = this->Distance(circle);
		float overlap = (distance - this->getSize() - circle->getSize()) * 0.5f;
		bool asymetric = this->getFixed() || circle->getFixed();
		
		float x_diff = this->getX() - circle->getX();
		float y_diff = this->getY() - circle->getY();

		if (asymetric)
		{
			if (!this->getFixed())
			{
				this->setX(this->getX() + overlap * (x_diff / distance) * 2.0f);
				this->setY(this->getY() + overlap * (y_diff / distance) * 2.0f);
			}
			else
			{
				circle->setX(circle->getX() + overlap * (x_diff / distance) * 2.0f);
				circle->setY(circle->getY() + overlap * (y_diff / distance) * 2.0f);
			}
		}
		else
		{
			this->setX(this->getX() + overlap * (x_diff / distance));
			this->setY(this->getY() + overlap * (y_diff / distance));

			circle->setX(circle->getX() + overlap * (x_diff / distance));
			circle->setY(circle->getY() + overlap * (y_diff / distance));
		}
		
		distance = this->Distance(circle);
		
		float normalX = (circle->getX() - this->getX()) / distance;
		float normalY = (circle->getY() - this->getY()) / distance;

		float tangentX = -normalX;
		float tangentY = normalY;

		float dpTanA = this->getX_speed() * tangentX + this->getY_speed() * tangentY;
		float dpTanB = circle->getX_speed() * tangentX + circle->getY_speed() * tangentY;
		
		float dpNormA = this->getX_speed() * normalX + this->getY_speed() * normalY;
		float dpNormB = circle->getX_speed() * normalX + circle->getY_speed() * normalY;

		float mA = (dpNormA * (this->getMass() - circle->getMass()) + 2.0f * circle->getMass() * dpNormB) / (this->getMass() + circle->getMass());
		float mB = (dpNormB * (circle->getMass() - this->getMass()) + 2.0f * circle->getMass()* dpNormA) / (this->getMass() + circle->getMass());

		if (asymetric)
		{
			if (!this->getFixed())
			{
				this->setX_speed(tangentX * dpTanA + normalX * mA);
				this->setY_speed(tangentY * dpTanA + normalY * mA);
			}
			else
			{
				circle->setX_speed(tangentX * dpTanB + normalX * mB);
				circle->setY_speed(tangentY * dpTanB + normalY * mB);
			}
		}
		else
		{
			this->setX_speed(tangentX * dpTanA + normalX * mA);
			this->setY_speed(tangentY * dpTanA + normalY * mA);
		
			circle->setX_speed(tangentX * dpTanB + normalX * mB);
			circle->setY_speed(tangentY * dpTanB + normalY * mB);
		}
	
	}

	void CollisionPolygon(Body* body){ }

	void Gravity(Body* body)
	{
		float G = 9.81;
		float distance = this->Distance(body);
		float mass_normal = (this->getMass() * body->getMass()) / pow(distance, 2); 

		float attraction = G * mass_normal;

		this->setX_speed(this->getX_speed() + attraction * (body->getX() - this->getX()) / distance);
		this->setY_speed(this->getY_speed() + attraction * (body->getY() - this->getY()) / distance);

		body->setX_speed(body->getX_speed() - attraction * (body->getX() - this->getX()) / distance);
		body->setY_speed(body->getY_speed() - attraction * (body->getY() - this->getY()) / distance);
		//body->setY_speed = body->y_speed - attraction * (body->x - this->x) / distance;
	}
};