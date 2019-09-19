//
// Created by bswenson3 on 11/9/16.
//
#pragma once

#include <SFML/Graphics.hpp>

class GameObject {
public:
	GameObject();
	virtual bool load(const std::string& filename) = 0;

	virtual void draw(sf::RenderWindow& window) = 0;

	virtual sf::IntRect update(sf::IntRect rect) { return rect; };

	virtual void setPosition(float x, float y) = 0;

	virtual void move(sf::Vector2f) = 0;

	virtual sf::Vector2f getPosition() const = 0;

	virtual float getHeight() const = 0;

	virtual float getWidth() const = 0;

	virtual void setScale(float scale) = 0;

	float m_speed = 240.0f;
	bool m_left = false;
	bool m_right = false;
	bool m_up = false;
	bool m_down = false;

	sf::Sprite m_sprite;
	sf::Texture m_texture;
	std::string m_filename;
	bool m_valid = false;
	sf::IntRect rectSourceSprite;

private:
	
};

