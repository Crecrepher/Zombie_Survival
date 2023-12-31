#pragma once
#include "SpriteGo.h"
#include "ObjectPool.h"

class Player;
class SoundGo;

class Zombie : public SpriteGo
{
public:
	enum class Types
	{
		None = -1,
		Bloater,
		Chaser,
		Crawler,
	};

	static const int TotalTypes = 3;

protected:
	Types zombieType;


	sf::Vector2f look;
	sf::Vector2f direction;
	float speed = 0.f;
	int maxHp = 0;
	int hp = 0;
	int damage = 0;
	float attackRate = 0.f;
	float attackTimer = 0.f;
	float crawlDash = 0.f;

	Player* player = nullptr;
	SoundGo* hitedSound = nullptr;
public:
	Zombie(const std::string n = "");
	virtual~Zombie()override;

	virtual void Init()override;
	virtual void Reset()override;
	virtual void Release() override;

	virtual void Update(float dt)override;
	virtual void Draw(sf::RenderWindow& window)override;

	void SetType(Types t);
	Types GetType() const;

	void SetPlayer(Player* player);
	void SetSound(SoundGo* hitSound) { hitedSound = hitSound; }

	void OnHitBullet(int damage);
};

