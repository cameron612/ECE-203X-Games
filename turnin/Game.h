//
// Created by bswenson3 on 11/9/16.
//
#pragma once

#include <SFML/Graphics.hpp>
#include "RegularGameObject.h"
#include "AnimatedGameObject.h"
#include "GameObject.h"
#include <vector>

class Game {
public:
	//use default screen size
	Game();
	//run the game
	void run();

private:
	void processEvents();
	//update the game objects
	void update(sf::Time deltaT);
	//draw the scene
	void render();
	//handle input from the user
	void handlePlayerInput(sf::Keyboard::Key key, bool isDown);

	sf::Clock clock2;
	sf::Time dt2;

	sf::RenderWindow m_window;

	std::vector<GameObject*> sprites = {new RegularGameObject, new AnimatedGameObject, new AnimatedGameObject , new RegularGameObject, new RegularGameObject};
};

