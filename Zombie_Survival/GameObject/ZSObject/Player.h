#pragma once
#include "SpriteGo.h"
#include "Gun.h"

class SoundGo;

class Player :public SpriteGo
{
protected:
	sf::Vector2f direction;
	sf::Vector2f look;
	
	float invincibility = 0;

	sf::FloatRect wallBounds;
	sf::Vector2f wallBoundsLT;
	sf::Vector2f wallBoundsRB;

	int maxHp = 100;
	int hp = 0;

	std::vector<Gun*> gunArray;
	int currentGunIndex = 0;
	SoundGo* hitedSound = nullptr;

public:
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
	void SetSound(SoundGo* hitSound) { hitedSound = hitSound; }

	int GetHp();
	int GetHpBarLength();
	const int GetAmmo() const;
	bool isAlive = false;

	void OnHitted(int damdge);
	void OnDie();


	void AddGun(Gun::Types type);
	const float GetReloadTimer() const;
	const ReloadStatus GetReloadStatus() const;
	void SetReloadStatus(ReloadStatus status);
	std::stringstream GetAmmoInfo();

	void ItemHealEat(int hp);
	void ItemAmmoEat(int ammo) { gunArray[0]->ammo += ammo; }
	void HealthUp() { maxHp += 20; ItemHealEat(20);}
	void SpeedUp() { speed += 30; }

};

