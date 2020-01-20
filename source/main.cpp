#include <SFML/Graphics.hpp>
#include "../include/System.cpp"
#include <iostream> 
#include <math.h>

using namespace sf;

int main()
{

	
	System BoxSystem("Box System", false, 0, 0.0f, 1000, 800);

	BoxSystem.addDot(Dot(200, 200, "A", 10, 10, 0, 0, 0.05));
	BoxSystem.addDot(Dot(200, 300, "A", 10, 10, 0, 0, 0.05));
	BoxSystem.addDot(Dot(300, 300, "A", 10, 10, 0, 0, 0.05));
	BoxSystem.addDot(Dot(300, 200, "A", 10, 10, 0, 0, 0.05));

	double stiffness = 10.0f;

	BoxSystem.addConstraint(Constraint(BoxSystem.getDot(0),BoxSystem.getDot(1),stiffness));
	BoxSystem.addConstraint(Constraint(BoxSystem.getDot(1),BoxSystem.getDot(2),stiffness));
	BoxSystem.addConstraint(Constraint(BoxSystem.getDot(2),BoxSystem.getDot(3),stiffness));
	BoxSystem.addConstraint(Constraint(BoxSystem.getDot(3),BoxSystem.getDot(0),stiffness));

	BoxSystem.addConstraint(Constraint(BoxSystem.getDot(2),BoxSystem.getDot(0),stiffness));

	BoxSystem.Render();

}
