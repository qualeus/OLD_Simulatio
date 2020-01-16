#include <SFML/Graphics.hpp>
#include "../include/System.cpp"
#include <iostream> 
#include <math.h>

using namespace sf;

int main()
{    
    /*System TestSystem("Test System", false, 0, 0.5f, 1000, 800);
    
    TestSystem.addDot(Dot(100, 100, "A", 10, 10, 0, 0, 0.05));
    TestSystem.addDot(Dot(100, 200, "B", 10, 10, 0, 0, 0.05));
    TestSystem.addDot(Dot(200, 200, "C", 10, 10, 0, 0, 0.05));
    TestSystem.addDot(Dot(200, 100, "D", 10, 10, 0, 0, 0.05));

    //TestSystem.addDot(Dot(200, 0, "x", 10, 10, 0, 0, 0.05, true, true));

	TestSystem.addConstraint(Constraint(TestSystem.getDot(0), TestSystem.getDot(1)));
	TestSystem.addConstraint(Constraint(TestSystem.getDot(1), TestSystem.getDot(2)));
	 
	TestSystem.addConstraint(Constraint(TestSystem.getDot(2), TestSystem.getDot(3)));
	TestSystem.addConstraint(Constraint(TestSystem.getDot(0), TestSystem.getDot(3)));
	TestSystem.addConstraint(Constraint(TestSystem.getDot(0), TestSystem.getDot(2)));

	//TestSystem.addConstraint(Constraint(TestSystem.getDot(1), TestSystem.getDot(3)));

	//TestSystem.addConstraint(Constraint(TestSystem.getDot(0), TestSystem.getDot(4)));


    TestSystem.Render();
    
    return 0;
}

/*
	# Base Demo System:
	System BaseSystem("Base System", false, 0, 0, 1000, 800);

    BaseSystem.add(Body(100, 100, "A", 10, 50, 0, 0, 0.05, true, true, Color::Red));
    BaseSystem.add(Body(300, 200, "B", 10, 50, 0, 0, 0.05));
    BaseSystem.add(Body(700, 500, "C", 10, 50, 0, 0, 0.05));
    BaseSystem.add(Body(900, 100, "D", 10, 50, 0, 0, 0.05));
    BaseSystem.add(Body(900, 300, "E", 10, 50, 0, 0, 0.05));
	BaseSystem.Render();
*/

/*
	# Capacity Demo System:
	System CapacitySystem("Capacity System", false, 0, 0, 1000, 800);

    for (int i= 0; i<100; i++)
    {
    	double Mass = 10 + rand() % 50;
    	CapacitySystem.add(Body(rand() % 1000, rand() % 800, "RandBody", Mass, Mass, 0, 0, 0.05));
    }
    CapacitySystem.Render();
 */

 /*
	# Gravity Demo System: 
	System GravitySystem("Gravity System", true, 0, 0, 1000, 800);

    GravitySystem.add(Body(500, 160, "def", 5, 10, 15, 0, 0));
    GravitySystem.add(Body(500, 100, "def", 5, 20, 10, 0, 0));
    GravitySystem.add(Body(500, 400, "def", 1000, 50, 0, 0, 0, true, false, Color::Blue));
    GravitySystem.Render();
*/	

/*
	# In work (Solar system)
	System SolarSystem("Solar System", true, 0, 0, 1400, 1000);

	SolarSystem.add(Body(850, 500, "Mercury", 10, 10, 0, 10, 0, false, false, Color(224, 219, 195)));
	SolarSystem.add(Body(900, 500, "Venus", 0, 20, 0, 0, 0, false, false, Color(204, 198, 94)));
	SolarSystem.add(Body(1050, 500, "Moon", 4, 10, 0, 17, 0, false, false, Color(193, 204, 217)));
	SolarSystem.add(Body(1000, 500, "Earth", 20, 25, 0, 20, 0, false, false, Color(33, 118, 222)));
	SolarSystem.add(Body(1100, 500, "Mars", 0, 20, 0, 0, 0, false, false, Color(224, 121, 65)));
	SolarSystem.add(Body(1128, 500, "Deimos", 0, 5, 0, 0, 0, false, false, Color(168, 161, 136)));
	SolarSystem.add(Body(1140, 500, "Phobos", 0, 5, 0, 0, 0, false, false, Color(94, 91, 77)));
	SolarSystem.add(Body(1200, 500, "Jupiter", 0, 45, 0, 0, 0, false, false, Color(224, 214, 164)));
	SolarSystem.add(Body(1300, 500, "Saturn", 0, 35, 0, 0, 0, false, false, Color(255, 243, 184)));
	SolarSystem.add(Body(1380, 500, "Uranus", 0, 30, 0, 0, 0, false, false, Color(29, 173, 245)));
	SolarSystem.add(Body(500, 500, "Sun", 1000, 100, 0, 0, 0, true, false, Color(252, 186, 3)));
	SolarSystem.Render();
*/