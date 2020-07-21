#include <iostream>

#include "../include/Renderer.hpp"
#include "../include/Quadtree.hpp"

int main()
{

	Renderer test = Renderer("Test", false, 0.0f, 0.1f);

	test.system.addCorpse(phy::Circle(500, 100, 40, 5, 1, 0, 0, false, false, sf::Color::Red));

	test.system.addCorpse(phy::Circle(700, 100, 40, 5, 1, 0, 0, false, false, sf::Color::Green));

	
	for (int i = 0; i < 300; ++i) { test.system.addCorpse(phy::Circle(rand() % 500 + 250, rand() % 500, rand() % 10 + 15, 5, 1, 0, 0, false, false, sf::Color::Blue)); }

	for (int i = 0; i < 10; ++i) { test.system.addCorpse(phy::Circle(100*i, 700, 50, 5, 1, 0, 0, true, false)); }
	for (int i = 0; i < 10; ++i) { test.system.addCorpse(phy::Circle(0, -300 + 100*i, 50, 5, 1, 0, 0, true, false)); }
	for (int i = 0; i < 10; ++i) { test.system.addCorpse(phy::Circle(900, -300 + 100*i, 50, 5, 1, 0, 0, true, false)); }
	
	test.Render();
 	return 0;
}