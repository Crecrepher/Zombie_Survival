#pragma once
#include "SpriteGo.h"
#include "ObjectPool.h"
#include "Bullet.h"

class Bullet;

class Player :public SpriteGo
{
protected:
	sf::Vector2f direction;
	sf::Vector2f look;
	float speed;
	float invincibility = 0;
	sf::Vector2f mapTop;//
	sf::Vector2f mapBot;//

	sf::FloatRect wallBounds;
	sf::Vector2f wallBoundsLT;
	sf::Vector2f wallBoundsRB;

	int maxHp = 100;
	int hp = 0;

	ObjectPool<Bullet> poolBullets;
	int maxAmmo = 900;
	int ammo = 0;
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

	void Ouch(float dt);
	int GetHp();
	const int GetAmmo() const;

	bool isAlive = false;

	void OnHitted(int damdge);
	void OnDie();

	void ItemHealEat(int hp) { this->hp += hp; }
	void ItemAmmoEat(int ammo) { this->ammo += ammo; }
};

