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
	
	float invincibility = 0;

	sf::FloatRect wallBounds;
	sf::Vector2f wallBoundsLT;
	sf::Vector2f wallBoundsRB;

	ObjectPool<Bullet> poolBullets;

	int hp = 0;
	int maxAmmo = 300;
	int ammo = 0;
public:
	int maxHp = 100;
	float speed;
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
	int GetHpBarLength();
	const int GetAmmo() const;

	bool isAlive = false;

	void OnHitted(int damdge);
	void OnDie();

	void ItemHealEat(int hp);
	void ItemAmmoEat(int ammo) { this->ammo += ammo; }
	void HealthUp() { maxHp += 20; ItemHealEat(20);}
	void SpeedUp() { speed += 30; }
};

