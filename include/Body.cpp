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

	bool fixed;
	bool etherial;

	Body(float x, float y, string name = "default", bool fixed = false, bool etherial = false)
	{
		this->x = x;
		this->y = y;
		this->name = name;
		this->fixed = fixed;
		this->etherial = etherial;
	}
};