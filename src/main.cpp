#include <iostream>

#include "../include/Renderer.hpp"

int main()
{

	Renderer test = Renderer(0.0f, 0.0f, 800.0f, 1200.0f, 1.0f, "Test", false, 0.0f, 1.0f, 2000.0f, 2000.0f);
	
	for (int i = 0; i < 200; ++i) { test.system.addCorpse(phy::Circle(rand() % 500 + 250, rand() % 500, rand() % 20 + 20, 1.0f, 2, 0, 0, false, false, false, sf::Color::Blue)); }

	/* Concave Shape / Simplify?
	test.system.addCorpse(phy::Polygon({{850, -350},{950, -350},{950, 750},{-50, 750},{-50, -350},{50, -350},{50, 650},{850, 650}}, 5, 1, 10, -50, true, false, C_NEPHRITIS));
	*/
	test.system.addCorpse(phy::Polygon({{750, 750},{950, 750},{950, -350},{750, -350}}, 5, 1, 10, -50, true, true, false, C_NEPHRITIS));
	test.system.addCorpse(phy::Polygon({{-150, 750},{50, 750},{50, -350},{-150, -350}}, 5, 1, 10, -50, true, true, false, C_NEPHRITIS));
	test.system.addCorpse(phy::Polygon({{-150, 950},{950, 950},{950, 750},{-150, 750}}, 5, 1, 10, -50, true, true, false, C_NEPHRITIS));

	test.system.addCorpse(phy::Polygon({{600, 50},{720, 90},{820, -150},{590, -10}}, 5, 1, 10, -50, true, false, false, sf::Color::Green));
	test.system.addCorpse(phy::Polygon({{0, 0},{0, 200},{200, 200},{200, 0}}, 5, 1, 10, -50, true, false, false, C_CARROT));



	//for (int i = 0; i < 10; ++i) { test.system.addCorpse(phy::Circle(100*i, 700, 50, 5, 1, 0, 0, true, false)); }
	//for (int i = 0; i < 10; ++i) { test.system.addCorpse(phy::Circle(0, -300 + 100*i, 50, 5, 1, 0, 0, true, false)); }
	//for (int i = 0; i < 10; ++i) { test.system.addCorpse(phy::Circle(900, -300 + 100*i, 50, 5, 1, 0, 0, true, false)); }
	
	test.Render();
 	return 0;
}