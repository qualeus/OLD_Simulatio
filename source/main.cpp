#include <SFML/Graphics.hpp>
#include "../include/System.cpp"
#include <iostream> 
#include <math.h>

using namespace sf;

int main()
{
/*
	System TestSystem("Test System", false, 0, 0.0f, 1000, 800);

	TestSystem.addDot(Dot(200, 200, "A", 10, 10, 0, 0, 0.05));
	TestSystem.addDot(Dot(200, 300, "A", 10, 10, 0, 0, 0.05));
	TestSystem.addDot(Dot(300, 300, "A", 10, 10, 0, 0, 0.05));
	TestSystem.addDot(Dot(300, 200, "A", 10, 10, 0, 0, 0.05));

	TestSystem.addConstraint(Constraint(TestSystem.getDot(0),TestSystem.getDot(1),0.3f));
	TestSystem.addConstraint(Constraint(TestSystem.getDot(1),TestSystem.getDot(2),0.3f));
	TestSystem.addConstraint(Constraint(TestSystem.getDot(2),TestSystem.getDot(3),0.3f));
	TestSystem.addConstraint(Constraint(TestSystem.getDot(3),TestSystem.getDot(0),0.3f));

	TestSystem.addConstraint(Constraint(TestSystem.getDot(2),TestSystem.getDot(0),0.3f));
	//TestSystem.addConstraint(Constraint(TestSystem.getDot(3),TestSystem.getDot(1)));


	//TestSystem.addDot(Dot(300, 100, "Fixed", 10, 5, 0, 0, 0.05, 1, 1, Color::Red));
	//TestSystem.addConstraint(Constraint(TestSystem.getDot(4),TestSystem.getDot(0)));


	for (int i=0; i<10; i++)
	{
		//TestSystem.addDot(Dot(300, 100 - 30 * i, "Fixed", 10, 5, 0, 0, 0.05));
	}

	for (int i=0; i<10; i++)
	{
		//TestSystem.addConstraint(Constraint(TestSystem.getDot(3+i),TestSystem.getDot(4+i), 0.3f, 30));
	}

	TestSystem.Render();
	
	}
*/


    System RopeSystem("Rope System", false, 0, 1.0f, 1000, 800);
    
    RopeSystem.addDot(Dot(100, 200, "Fixed", 10, 10, 0, 0, 0.05, 1, 1, Color::Red));

    int ropel = 20;

    for (int i=0; i < ropel; i++)
    {
		RopeSystem.addDot(Dot(100, 230 - i * 10, "rope", 10, 0, 0, 0, 0.05));
    }

    RopeSystem.addDot(Dot(400, 200, "Fixed", 10, 10, 0, 0, 0.05, 1, 1, Color::Red));
    //RopeSystem.addDot(Dot(400, 200, "Fixed", 100, 100, 0, 0, 0.05, 0, 1, Color::Green));
    
    for (int i=0; i < ropel +1 ; i++)
    {
		RopeSystem.addConstraint(Constraint(RopeSystem.getDot(i), RopeSystem.getDot(i+1),0.3f,30));
    }


    RopeSystem.Render();
    
    return 0;
}



/*
	// # Base Demo System:
	System BaseSystem("Base System", false, 0, 0, 1000, 800);

    BaseSystem.addDot(Dot(100, 100, "A", 10, 50, 0, 0, 0.05, true, true, Color::Red));
    BaseSystem.addDot(Dot(300, 200, "B", 10, 50, 0, 0, 0.05));
    BaseSystem.addDot(Dot(700, 500, "C", 10, 50, 0, 0, 0.05));
    BaseSystem.addDot(Dot(900, 100, "D", 10, 50, 0, 0, 0.05));
    BaseSystem.addDot(Dot(900, 300, "E", 10, 50, 0, 0, 0.05));
	BaseSystem.Render();
}
*/


/*
	// # Capacity Demo System:
	System CapacitySystem("Capacity System", false, 0, 0, 1000, 800);

    for (int i= 0; i<100; i++)
    {
    	double Mass = 10 + rand() % 50;
    	CapacitySystem.addDot(Dot(rand() % 1000, rand() % 800, "RandBody", Mass * Mass, Mass, 0, 0, 0.05));
    }
    CapacitySystem.Render();
 
 }
 */

 /*
	//# Gravity Demo System: 
	System GravitySystem("Gravity System", true, 0, 0, 1000, 800);

    GravitySystem.addDot(Dot(500, 160, "def", 5, 10, 15, 0, 0));
    GravitySystem.addDot(Dot(500, 100, "def", 5, 20, 10, 0, 0));
    GravitySystem.addDot(Dot(500, 400, "def", 1000, 50, 0, 0, 0, true, false, Color::Blue));
    GravitySystem.Render();

}
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
