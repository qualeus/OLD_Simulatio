#include <SFML/Graphics.hpp>
#include "../include/System.cpp"
#include <iostream> 
#include <math.h>

using namespace sf;

int main()
{

/* ---------------------------------------------------------------------------------------------------- */
/*                                            Gravity System                                            */
/* ---------------------------------------------------------------------------------------------------- */

System GravitySystem("Gravity System", true, 0, 0, 1000, 800);

GravitySystem.addDot(Dot(500, 160, "def", 5, 10, 15, 0, 0));
GravitySystem.addDot(Dot(500, 100, "def", 5, 20, 10, 0, 0));
GravitySystem.addDot(Dot(500, 400, "def", 1000, 50, 0, 0, 0, true, true, Color::Blue));
GravitySystem.Render();
}
