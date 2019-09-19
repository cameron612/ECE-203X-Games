#pragma once
#include "c:\Users\coolc\Documents\Current Semester Stuff\ECE 2036\SkeletonCode\SkeletonCode\GameObject.h"
class AnimatedGameObject :
	public GameObject
{
public:
	AnimatedGameObject();
	virtual bool load(const std::string& filename);

	virtual void draw(sf::RenderWindow& window);

	virtual sf::IntRect update(sf::IntRect rect);

	virtual void setPosition(float x, float y);

	virtual void move(sf::Vector2f);

	virtual sf::Vector2f getPosition() const;

	virtual float getHeight() const;

	virtual float getWidth() const;

	virtual void setScale(float scale);
	float m_speed = 0.0f;
	sf::IntRect rectSourceSprite;

private:
	/*sf::Sprite m_sprite;
	sf::Texture m_texture;
	std::string m_filename;
	bool m_valid = false;*/
};

