#pragma once
#include "SpriteGo.h"
#include "ObjectPool.h"
#include "Bullet.h"

class Bullet;

enum class ReloadStatus
{
	NONE = -1,
	START,
	END,
};

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
	int	maxMagazine = 15;
	int magazine = 0;
	ReloadStatus reloadStatus = ReloadStatus::NONE;
	float reloadRate = 2.f;
	float reloadRateTimer = 0.f;
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
	const int GetMagazine() const;
	const ReloadStatus GetReload() const;
	void SetReloadStatus(ReloadStatus status);

	//Ω∫≈»
	void SetMaxHp(int hp);
	void SetSpeed(float speed);
	void SetMaxMagazine(int magazine);
	void SetReloadRate(float reloadRate);
	
	bool isAlive = false;

	void OnHitted(int damdge);
	void OnDie();
};

