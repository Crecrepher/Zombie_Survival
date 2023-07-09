#pragma once
#include "SpriteGo.h"
#include "Gun.h"

class SoundGo;

class Player :public SpriteGo
{
protected:
	sf::Vector2f direction;
	sf::Vector2f look;
	
	bool bloaterBonk = false;
	sf::Vector2f bonkDir = {0.f,0.f};
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
	bool isAlive = false;

	void OnHitted(int damdge);
	void OnHittedByBloat(sf::Vector2f bonkDir);
	void OnDie();

	void AddGun(Gun::Types type);
	const float GetReloadTimer() const;
	const ReloadStatus GetReloadStatus() const;
	std::stringstream GetAmmoInfo();
	int GetCurrentGun() const { return currentGunIndex; }

	void SetReloadStatus(ReloadStatus status);
	void SetGunSound(SoundGo* shootSound, SoundGo* shootFailSound, SoundGo* reloadSound);

	void ItemHealEat(int hp);
	void ItemAmmoEat(int ammo) { gunArray[0]->ammo += ammo; gunArray[1]->ammo += ammo/2; }
	void HealthUp() { maxHp += 20; ItemHealEat(20);}
	void SpeedUp() { speed += 30; }
	void GunUp(int upCase);
};

