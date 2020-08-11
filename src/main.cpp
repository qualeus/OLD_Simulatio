#include <iostream>

#include "../include/Renderer.hpp"

int main()
{

	Renderer test = Renderer(0.0f, 0.0f, 800.0f, 1200.0f, 1.0f, "Test", false, 0.0f, 1.0f, 2000.0f, 2000.0f);

	test.system.addCorpse(phy::Circle(500, 100, 300, 5, 1, 0, 0, true, false, sf::Color::Red));
	test.system.addCorpse(phy::Polygon({{600, 50},{720, 90},{820, -150},{590, -10}}, 5, 1, 10, -50, true, false, sf::Color::Green));
	test.system.addCorpse(phy::Polygon({{-200, -200},{-200, 200},{200, 200},{200, -200}}, 5, 1, 10, -50, true, false, C_CARROT));


	for (int i = 0; i < 200; ++i) { test.system.addCorpse(phy::Circle(rand() % 500 + 250, rand() % 500, rand() % 20 + 20, 5, 2, 0, 0, false, false, sf::Color::Blue)); }

	for (int i = 0; i < 10; ++i) { test.system.addCorpse(phy::Circle(100*i, 700, 50, 5, 1, 0, 0, true, false)); }
	for (int i = 0; i < 10; ++i) { test.system.addCorpse(phy::Circle(0, -300 + 100*i, 50, 5, 1, 0, 0, true, false)); }
	for (int i = 0; i < 10; ++i) { test.system.addCorpse(phy::Circle(900, -300 + 100*i, 50, 5, 1, 0, 0, true, false)); }

	test.Render();
 	return 0;
}