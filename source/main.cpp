#include <SFML/Graphics.hpp>
#include "../include/System.cpp"
#include <iostream> 
#include <math.h>

using namespace sf;

int main()
{

    System SolarSystem("Solar System",0,0, 1000, 800);

    for (int i= 0; i<2; i++)
    {
    	SolarSystem.add(Body(rand() % 1000, rand() % 800, "def", 20, 50, 0, 0, 0.05));
    }
	
	SolarSystem.add(Body(rand() % 1000, rand() % 800, "def", 1000, 100, 0, 0, 0.05));

    SolarSystem.add(Body(200, 100, "def", 20, 50, 0, 0, 0, true, false, Color::Blue));
    SolarSystem.add(Body(400, 100, "def", 20, 50, 0, 0, 0, true, false, Color::Blue));

    /*
    SolarSystem.add(Body(100, 100, "A", 10, 50, 0, 0, 0.05));
    SolarSystem.add(Body(300, 200, "B", 10, 50, 0, 0, 0.05));
    SolarSystem.add(Body(700, 500, "C", 10, 50, 0, 0, 0.05));
    SolarSystem.add(Body(900, 100, "D", 10, 50, 0, 0, 0.05));
    SolarSystem.add(Body(900, 300, "E", 10, 50, 0, 0, 0.05));
    SolarSystem.add(Body(900, 300, "F", 10, 50, 0, 0, 0.05));
	*/

    SolarSystem.Render();

    return 0;
}