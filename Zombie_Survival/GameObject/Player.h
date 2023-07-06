#pragma once
#include "SpriteGo.h"
class Player :public SpriteGo
{
protected:
	sf::Vector2f direction;
	sf::Vector2f look;
	float speed;

	int maxHp = 100;
	int hp = 0;
	float invincibility = 0;
	sf::Vector2f mapTop;
	sf::Vector2f mapBot;

	sf::FloatRect wallBounds;
	sf::Vector2f wallBoundsLT;
	sf::Vector2f wallBoundsRB;


public:
	Player(const std::string id = "",const std::string n = "");

	virtual void SetPosition(float x, float y)override;
	virtual void SetPosition(const sf::Vector2f& p)override;

	virtual void Init()override;
	virtual void Reset()override;
	virtual void Release() override;

	virtual void Update(float dt)override;
	virtual void Draw(sf::RenderWindow& window)override;
	
	void SetWallBounds(const sf::FloatRect& bounds);

	void GetMap();
	void Ouch(float dt);
	float GetHp();

	bool isAlive = false;

	void OnHitted(int damdge);
	void OnDie();
};

