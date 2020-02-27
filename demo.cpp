
/* ---------------------------------------------------------------------------------------------------- */
/*                                              Dots System                                             */
/* ---------------------------------------------------------------------------------------------------- */

System BaseSystem("Base System", false, 0, 0, 1000, 800);

BaseSystem.addDot(Dot(100, 100, "A", 10, 50, 0, 0, 0.05, true, true, Color::Red));
BaseSystem.addDot(Dot(300, 200, "B", 10, 50, 0, 0, 0.05));
BaseSystem.addDot(Dot(700, 500, "C", 10, 50, 0, 0, 0.05));
BaseSystem.addDot(Dot(900, 100, "D", 10, 50, 0, 0, 0.05));
BaseSystem.addDot(Dot(900, 300, "E", 10, 50, 0, 0, 0.05));
BaseSystem.Render();


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

/* ---------------------------------------------------------------------------------------------------- */
/*                                            Gravity System                                            */
/* ---------------------------------------------------------------------------------------------------- */

System GravitySystem("Gravity System", true, 0, 0, 1000, 800);

GravitySystem.addDot(Dot(500, 160, "def", 5, 10, 15, 0, 0));
GravitySystem.addDot(Dot(500, 100, "def", 5, 20, 10, 0, 0));
GravitySystem.addDot(Dot(500, 400, "def", 1000, 50, 0, 0, 0, true, true, Color::Blue));
GravitySystem.Render();

/* ---------------------------------------------------------------------------------------------------- */
/*                                              Solar System                                            */
/* ---------------------------------------------------------------------------------------------------- */

	System SolarSystem("Solar System", false, 0, 0, 1400, 1000);

	SolarSystem.addDot(Dot(850, 500, "Mercury", 10, 10, 0, 0, 0, false, false, Color(224, 219, 195)));
	SolarSystem.addDot(Dot(900, 500, "Venus", 10, 20, 0, 0, 0, false, false, Color(204, 198, 94)));
	SolarSystem.addDot(Dot(1050, 500, "Moon", 4, 10, 0, 0, 0, false, false, Color(193, 204, 217)));
	SolarSystem.addDot(Dot(1000, 500, "Earth", 20, 25, 0, 0, 0, false, false, Color(33, 118, 222)));
	SolarSystem.addDot(Dot(1100, 500, "Mars", 10, 20, 0, 0, 0, false, false, Color(224, 121, 65)));
	SolarSystem.addDot(Dot(1128, 500, "Deimos", 10, 5, 0, 0, 0, false, false, Color(168, 161, 136)));
	SolarSystem.addDot(Dot(1140, 500, "Phobos", 10, 5, 0, 0, 0, false, false, Color(94, 91, 77)));
	SolarSystem.addDot(Dot(1200, 500, "Jupiter", 10, 45, 0, 0, 0, false, false, Color(224, 214, 164)));
	SolarSystem.addDot(Dot(1300, 500, "Saturn", 10, 35, 0, 0, 0, false, false, Color(255, 243, 184)));
	SolarSystem.addDot(Dot(1380, 500, "Uranus", 10, 30, 0, 0, 0, false, false, Color(29, 173, 245)));
	SolarSystem.addDot(Dot(500, 500, "Sun", 1000, 100, 0, 0, 0, true, false, Color(252, 186, 3)));
	SolarSystem.Render();

/* ---------------------------------------------------------------------------------------------------- */
/*                                                Box System                                            */
/* ---------------------------------------------------------------------------------------------------- */

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

/* ---------------------------------------------------------------------------------------------------- */
/*                                              Rope System                                             */
/* ---------------------------------------------------------------------------------------------------- */

	System RopeSystem("Rope System", false, 0, 1.0f, 1000, 800);

	RopeSystem.addDot(Dot(100, 200, "Fixed", 10, 10, 0, 0, 0.05, 1, 1, Color::Red));

	int ropel = 20;

	for (int i=0; i < ropel; i++)
	{
		RopeSystem.addDot(Dot(100, 230 - i * 10, "rope", 10, 0, 0, 0, 0.05));
	}

	//RopeSystem.addDot(Dot(400, 200, "Fixed", 10, 10, 0, 0, 0.05, 1, 1, Color::Red));
	RopeSystem.addDot(Dot(400, 200, "Fixed", 100, 100, 0, 0, 0.05, 0, 1, Color::Green));

	for (int i=0; i < ropel +1 ; i++)
	{
	RopeSystem.addConstraint(Constraint(RopeSystem.getDot(i), RopeSystem.getDot(i+1),0.3f,30));
	}


	RopeSystem.Render();


/* ---------------------------------------------------------------------------------------------------- */
/*                                             Textile System                                           */
/* ---------------------------------------------------------------------------------------------------- */
	
	int stiffness = 1.0f;
	int xSize = 10;
	int ySize = 10;

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
