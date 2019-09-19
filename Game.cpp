//
// Created by bswenson3 on 11/9/16.
//
#include <iostream>
#include "Game.h"
#include <Windows.h>

Game::Game() :
	m_window(sf::VideoMode(640, 480), "Cool Game")
{
	//load the players
	for (int i = 0; i < sprites.size(); ++i) {
		if (i == 0) {
			sprites[i]->load("content/tree.png");
			sprites[i]->setScale(.1f);
		}
		if (i == 1) {
			sprites[i]->load("content/cars.jpg");
			sprites[i]->setPosition(400.0f, 0.0f);
			sprites[i]->setScale(.9f);
			sprites[i]->rectSourceSprite = sf::IntRect(108, 60, 60, 130);
			sprites[i]->m_sprite.setTextureRect(sprites[i]->rectSourceSprite);
		}
		if (i == 2) {
			sprites[i]->load("content/cops.jpg");
			sprites[i]->setPosition(150.0f, 0.0f);
			sprites[i]->setScale(.9f);
			sprites[i]->rectSourceSprite = sf::IntRect(140, 65, 55, 120);
			sprites[i]->m_sprite.setTextureRect(sprites[i]->rectSourceSprite);
		}
		if (i == 3) {
			sprites[i]->load("content/tree.png");
			sprites[i]->setScale(.1f);
			sprites[i]->setPosition(0.0f, 150.0f);
		}
		if (i == 4) {
			sprites[i]->load("content/tree.png");
			sprites[i]->setScale(.1f);
			sprites[i]->setPosition(0.0f, 400.0f);
		}

		
	}
}

void Game::run() {
	sf::Clock clock;

	while (m_window.isOpen()) {
		sf::Time deltaT = clock.restart();
		processEvents();
		update(deltaT);
		render();
	}
}

void Game::processEvents() {
	sf::Event event;
	while (m_window.pollEvent(event)) {
		switch (event.type) {
		case sf::Event::KeyPressed:
			//handle key down here
			handlePlayerInput(event.key.code, true);
			break;
		case sf::Event::KeyReleased:
			handlePlayerInput(event.key.code, false);
			break;

		case sf::Event::Closed:
			m_window.close();
			break;
		}
	}
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isDown) {
	if (key == sf::Keyboard::Left) {
		for (int i = 0; i < sprites.size(); ++i) {
			sprites[i]->m_left = isDown;
		}
	}
	if (key == sf::Keyboard::Right) {
		for (int i = 0; i < sprites.size(); ++i) {
			sprites[i]->m_right = isDown;
		}
	}

	if (key == sf::Keyboard::Up) {
		for (int i = 0; i < sprites.size(); ++i) {
			sprites[i]->m_up = isDown;
		}
	}

	if (key == sf::Keyboard::Down) {
		for (int i = 0; i < sprites.size(); ++i) {
			sprites[i]->m_down = isDown;
		}
	}

	if (key == sf::Keyboard::Space) {
		for (int i = 0; i < sprites.size(); ++i) {
			sprites[i]->rectSourceSprite = sprites[i]->update(sprites[i]->rectSourceSprite);
			sprites[i]->m_sprite.setTextureRect(sprites[i]->rectSourceSprite);
			//render();
			//Sleep(500);
		}
	}
}

//use time since last update to get smooth movement
void Game::update(sf::Time deltaT) {
	//Look a vector class!
	for (int i = 0; i < sprites.size(); ++i) {
		sf::Vector2f movement(0.0f, 0.0f);
		if (sprites[i]->m_up)
			movement.y -= sprites[i]->m_speed;
		if (sprites[i]->m_down)
			movement.y += sprites[i]->m_speed;
		if (sprites[i]->m_left)
			movement.x -= sprites[i]->m_speed;
		if (sprites[i]->m_right)
			movement.x += sprites[i]->m_speed;

		sprites[i]->move(movement * deltaT.asSeconds());
	}

}

void Game::render() {
	m_window.clear();
	dt2 = clock2.getElapsedTime();
	for (int i = 0; i < sprites.size(); ++i) {
		if (dt2.asSeconds() >= 1.0f) {
			sprites[i]->rectSourceSprite = sprites[i]->update(sprites[i]->rectSourceSprite);
			sprites[i]->m_sprite.setTextureRect(sprites[i]->rectSourceSprite);
			clock2.restart();
		}
		sprites[i]->draw(m_window);
	}

	m_window.display();
}