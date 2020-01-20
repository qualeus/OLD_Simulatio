#include <SFML/Graphics.hpp>
#include "../include/System.cpp"
#include <iostream> 
#include <math.h>

using namespace sf;

int main()
{

	int stiffness = 1.0f;
	int xSize = 13;
	int ySize = 13;

	System TextileSystem("Box System", false, 0, 0.3f, 1000, 800);

	for (int x=0; x < xSize; x++)
	{
		for (int y=0; y < ySize; y++)
		{
			if (y == 0)
			{
				TextileSystem.addDot(Dot(300 + 30*x, 50 + 30*y, "a", 10, 10, 0, 0, 0.05, true, true, Color::Red));
			}
			else
			{
				TextileSystem.addDot(Dot(300 + 30*x, 50 + 30*y, "p", 1, 5, 0, 0, 0.05, false, true, Color::White));
			}
		}
	}

	for (int x=0; x < xSize; x++)
	{
		for (int y=0; y < ySize; y++)
		{
			if (x < xSize -1)
			{
				TextileSystem.addConstraint(Constraint(TextileSystem.getDot(xSize*y + x),TextileSystem.getDot(xSize*y + x+1),stiffness));
			}

			if (y < ySize -1)
			{
				TextileSystem.addConstraint(Constraint(TextileSystem.getDot(xSize*y + x),TextileSystem.getDot(xSize*(y+1)+x),stiffness));
			}			
		}
	}


	TextileSystem.Render();

}
