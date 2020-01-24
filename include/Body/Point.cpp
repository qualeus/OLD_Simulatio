#include "Body.cpp"

using namespace std;
using namespace sf;

class Point : public Body
{
private:
	float lx, ly;
	float friction

public:
	Point()
	:Body(0.0f, 0.0f, 1.0f)
	{
		this->lx = 0.0f;
		this->lx = 0.0f;
		this->friction = 0.0f;
	}

	Point(float x, float y, string name, float mass)
	:Body(x, y, mass, name)
	{
		this->lx = x;
		this->lx = y;
		this->friction = 0.0f;
	}

	Point(float x, float y, float dx, float dy, string name, float mass)
	:Body(x, y, mass, name)
	{
		this->lx = x + dx;
		this->lx = y + dy;
		this->friction = 0.0f;
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
			float dx = this->lx - this->x;
			float dy = this->ly - this->x

			if (abs(this->dx) < 0.01)
			{
				this->lx = this->x;
			}

			if (abs(this->dy) < 0.01)
			{
				this->dy = this->y;
			}

			tempx = this->x;
			tempy = this->y;

			this->x = this->x + dx * (1 - this->friction);
			this->y = this->y + dy * (1 - this->friction);

			this->lx = tempx;
 			this->ly = tempy;
			
		}
	}

}