#pragma once
#include "SpriteGo.h"
class Player :public SpriteGo
{
protected:
	sf::Vector2f direction;
	sf::Vector2f look;
	float speed;
	float hp;

	sf::Vector2f mapTop;
	sf::Vector2f mapBot;
public:
	Player(const std::string id = "",const std::string n = "");
	virtual ~Player() override;

	virtual void SetPosition(float x, float y)override;
	virtual void SetPosition(const sf::Vector2f& p)override;

	virtual void Init()override;
	virtual void Reset()override;
	virtual void Release() override;

	virtual void Update(float dt)override;
	virtual void Draw(sf::RenderWindow& window)override;

	void SetMapInfo();
	void Ouch(float dt);
	float GetHp();
};

