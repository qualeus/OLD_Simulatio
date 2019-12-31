#include <SFML/Graphics.hpp>
#include "../include/System.cpp"
#include <iostream> 
#include <math.h>

using namespace sf;

int main()
{
    /*
    System SolarSystem("Base System", false, 0, 0, 1000, 800);
    SolarSystem.add(Body(100, 100, "A", 10, 50, 0, 0, 0.05));
    SolarSystem.add(Body(300, 200, "B", 10, 50, 0, 0, 0.05));
    SolarSystem.add(Body(700, 500, "C", 10, 50, 0, 0, 0.05));
    SolarSystem.add(Body(900, 100, "D", 10, 50, 0, 0, 0.05));
    SolarSystem.add(Body(900, 300, "E", 10, 50, 0, 0, 0.05));
	*/

	System SolarSystem("Capacity System", false, 0, 0, 1000, 800);
    for (int i= 0; i<10; i++)
    {
    	double Mass = 10 + rand() % 50;
    	SolarSystem.add(Body(rand() % 1000, rand() % 800, "RandBody", Mass, Mass, 0, 0, 0.05));
    }
    
    /*
	System SolarSystem("Gravity System", true, 0, 0, 1000, 800);
    SolarSystem.add(Body(500, 160, "def", 5, 10, 15, 0, 0));
    SolarSystem.add(Body(500, 100, "def", 5, 20, 10, 0, 0));
    SolarSystem.add(Body(500, 400, "def", 1000, 50, 0, 0, 0, true, false, Color::Blue));
	*/
    
    

    SolarSystem.Render();

    return 0;
}