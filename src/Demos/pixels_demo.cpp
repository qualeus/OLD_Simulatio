#include <SFML/Graphics.hpp>
#include <SFML/Main.hpp>

int main()
{
	int ScreenWidth = 640;
	int ScreenHeight = 480;

	sf::RenderWindow window(sf::VideoMode(ScreenWidth, ScreenHeight), "Pixels Demo");

	while(window.isOpen()) {
		sf::Event e;
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				window.close();
			}
		}

		window.clear(sf::Color::Black);

		sf::Image image;
		image.create(ScreenWidth, ScreenHeight);

		for (int x = 0; x < ScreenWidth; x++) {
			for (int y = 0; y < ScreenHeight; y++) {
				image.setPixel(x, y, sf::Color(rand() % 255, rand() % 255, rand() % 255));
			}			
		}

		sf::Texture texture;
		texture.loadFromImage(image);

		sf::Sprite sprite(texture);

		window.draw(sprite);
		window.display();
	}
	return 0;
}