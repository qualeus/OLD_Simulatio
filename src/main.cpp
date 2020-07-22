#include <iostream>

#include "../include/Renderer.hpp"
#include "../include/Quadtree.hpp"

int main()
{

	Renderer test = Renderer("Test", false, 0.0f, 0.4f, 2000, 2000);

	test.system.addCorpse(phy::Circle(-100, 100, 40, 5, 1, 0, 0, false, false, sf::Color::Red));

	test.system.addCorpse(phy::Circle(100, 100, 40, 5, 1, 0, 0, false, false, sf::Color::Green));

	
	for (int i = 0; i < 400; ++i) { test.system.addCorpse(phy::Circle(rand() % 500 - 350, rand() % 500, rand() % 10 + 15, 5, 2, 0, 0, false, false, sf::Color::Blue)); }

	for (int i = 0; i < 10; ++i) { test.system.addCorpse(phy::Circle(100*i-450, 700, 50, 5, 1, 0, 0, true, false)); }
	for (int i = 0; i < 10; ++i) { test.system.addCorpse(phy::Circle(-450, -300 + 100*i, 50, 5, 1, 0, 0, true, false)); }
	for (int i = 0; i < 10; ++i) { test.system.addCorpse(phy::Circle(450, -300 + 100*i, 50, 5, 1, 0, 0, true, false)); }
	
	test.Render();
 	return 0;
}