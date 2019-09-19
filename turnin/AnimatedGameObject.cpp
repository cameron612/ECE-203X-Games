#include "AnimatedGameObject.h"
#include <Windows.h>
#include <string>

AnimatedGameObject::AnimatedGameObject() {
	this->rectSourceSprite = sf::IntRect(1, 1, 1, 1);
}

bool AnimatedGameObject::load(const std::string& filename) {
	if (this->m_texture.loadFromFile(filename)) {
		//rectSourceSprite = sf::IntRect(150, 90, 50, 90);
		this->m_filename = filename;
		this->m_sprite.setTexture(m_texture);// , rectSourceSprite);
		//this->m_sprite.setTextureRect(rectSourceSprite);
		this->m_valid = true;
		return true;
	}
	return false;
	//return GameObject::load(filename);
}

void AnimatedGameObject::draw(sf::RenderWindow& window) {
	GameObject::draw(window);
}

void AnimatedGameObject::move(sf::Vector2f loc) {
	GameObject::move(loc);
}

void AnimatedGameObject::setPosition(float x, float y) {
	GameObject::setPosition(x, y);
}

sf::Vector2f AnimatedGameObject::getPosition() const {
	return GameObject::getPosition();
}

float AnimatedGameObject::getHeight() const {
	return GameObject::getHeight();
}

float AnimatedGameObject::getWidth() const {
	return GameObject::getWidth();
}

void AnimatedGameObject::setScale(float scale) {
	GameObject::setScale(scale);
}

sf::IntRect AnimatedGameObject::update(sf::IntRect rect) {
	if ((rect.left) % 100 == 40) {
		if (rect.left >= 540)
			rect.left = 140;
		else
			rect.left += 100;
	}
	else if ((rect.left) % 10 == 8 || (rect.left) % 10 == 3) {
		if (rect.left >= 540)
			rect.left = 108;
		else
			rect.left += 115;
	}

	return rect;
}