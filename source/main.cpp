#include <SFML/Graphics.hpp>
#include "../include/System.cpp"
#include <iostream> 
#include <math.h>

using namespace sf;

int main()
{

	/* ---------------------------------------------------------------------------------------------------- */
	/*                                            Capacity System                                           */
	/* ---------------------------------------------------------------------------------------------------- */

	System CapacitySystem("Capacity System", false, 0, 0, 1000, 800);

	for (int i= 0; i<100; i++)
	{
		double Mass = 10 + rand() % 50;
		CapacitySystem.addDot(Dot(rand() % 1000, rand() % 800, "RandBody", Mass * Mass, Mass, 0, 0, 0.05));
	}
	CapacitySystem.Render();
}
