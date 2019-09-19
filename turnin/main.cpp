#include <SFML/Graphics.hpp>
#include "Game.h"

int main()
{
	Game game;
	game.run();
	return 0;
}

/*
#include <SFML/Graphics.hpp>

int main()
{
	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(shape);
		window.display();
	}

	return 0;
}*/
/*
#include <iostream>
#include <memory>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "resource_path.h"


int main() {

	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Window");
	sf::Font font;
	sf::Texture texture;
	if (!texture.loadFromFile(resource_path + "Mario.png")) {
		std::cout << "Unable to load bullet.png";
		return EXIT_FAILURE;
	}
	sf::Sprite sprite(texture);

	if (!font.loadFromFile(resource_path + "AlexandriaFLF.ttf")) {
		std::cout << "Unable to load AlexandriaFLF.ttf font" << std::endl;
		return EXIT_FAILURE;
	}
	sf::Text text("Hello SFML", font, 50);

	sf::Music music;
	if (!music.openFromFile(resource_path + "background.wav"))
		return EXIT_FAILURE;


	sf::SoundBuffer spaceGun;
	if (!spaceGun.loadFromFile(resource_path + "space_gun.wav")) {
		return EXIT_FAILURE;
	}
	sf::Sound spaceGunSound;
	spaceGunSound.setBuffer(spaceGun);

	sf::SoundBuffer explode;
	if (!explode.loadFromFile(resource_path + "ship_explode.wav")) {
		return EXIT_FAILURE;
	}
	sf::Sound shipExplodeSound;
	shipExplodeSound.setBuffer(explode);

	music.setLoop(true);
	music.play();

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Space) {
					spaceGunSound.play();
				}
				else if (event.key.code == sf::Keyboard::Q) {
					shipExplodeSound.play();
				}
				break;
			}

		}

		window.clear();
		window.draw(text);
		window.draw(sprite);

		window.display();
	}

	return EXIT_SUCCESS;
}*/