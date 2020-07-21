#include <iostream>

#include "../../include/Renderer.hpp"

int main()
{
	Renderer test = Renderer("Granular", false, 0.0f, 1.0f);

	test.system.addCorpse(phy::Circle(500, 100, 40, 5, 1, 0, 0, false, false, sf::Color::Red));

	for (int i = 0; i < 600; ++i) { test.system.addCorpse(phy::Circle(rand() % 500 + 250, rand() % 500, rand() % 20 + 5, 5, 2, 0, 0, false, false, sf::Color::Blue)); }

	for (int i = 0; i < 10; ++i) { test.system.addCorpse(phy::Circle(100*i, 700, 50, 5, 1, 0, 0, true, false)); }
	for (int i = 0; i < 10; ++i) { test.system.addCorpse(phy::Circle(0, -300 + 100*i, 50, 5, 1, 0, 0, true, false)); }
	for (int i = 0; i < 10; ++i) { test.system.addCorpse(phy::Circle(900, -300 + 100*i, 50, 5, 1, 0, 0, true, false)); }

	test.Render();
 	return 0;
}