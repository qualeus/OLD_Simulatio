#include <iostream>

#include "../include/Renderer.hpp"

int main()
{

	Renderer test = Renderer(0.0f, 0.0f, 800.0f, 1200.0f, 1.0f, "Test", false, 0.0f, 1.0f, 2000.0f, 2000.0f);

	test.system.addCorpse(phy::Circle(500, 100, 40, 5, 1, 0, 0, false, false, sf::Color::Red));
	std::vector<sf::Vector2f> pointsA = std::vector<sf::Vector2f>();
	pointsA.push_back({-100.0f, 50.0f});
	pointsA.push_back({20.0f, 90.0f});
	pointsA.push_back({120.0f, -150.0f});
	pointsA.push_back({-190.0f, -10.0f});
	test.system.addCorpse(phy::Polygon(pointsA, 5, 1, 10, -50, true, false, sf::Color::Green));

	std::vector<sf::Vector2f> pointsB = std::vector<sf::Vector2f>();
	pointsB.push_back({600, 200});
	pointsB.push_back({700, 200});
	pointsB.push_back({700, 300});
	pointsB.push_back({600, 300});
	test.system.addCorpse(phy::Polygon(pointsB, 5, 1, 10, -50, true, false, sf::Color::Green));


	for (int i = 0; i < 200; ++i) { test.system.addCorpse(phy::Circle(rand() % 500 + 250, rand() % 500, rand() % 20 + 20, 5, 2, 0, 0, false, false, sf::Color::Blue)); }

	for (int i = 0; i < 10; ++i) { test.system.addCorpse(phy::Circle(100*i, 700, 50, 5, 1, 0, 0, true, false)); }
	for (int i = 0; i < 10; ++i) { test.system.addCorpse(phy::Circle(0, -300 + 100*i, 50, 5, 1, 0, 0, true, false)); }
	for (int i = 0; i < 10; ++i) { test.system.addCorpse(phy::Circle(900, -300 + 100*i, 50, 5, 1, 0, 0, true, false)); }

	test.Render();
 	return 0;
}