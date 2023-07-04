#pragma once
#include "SpriteGo.h"
class Player :public SpriteGo
{
protected:
	sf::Vector2f direction;
	sf::Vector2f position;
	sf::Vector2f look;
	float speed;
public:
	Player(const std::string id = "",const std::string n = "");

	virtual void Init()override;
	virtual void Reset()override;
	virtual void Release() override;

	virtual void Update(float dt)override;
	virtual void Draw(sf::RenderWindow& window)override;
};

