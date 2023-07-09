#pragma once
#include "SpriteGo.h"
#include "ObjectPool.h"

class Bullet;
class SoundGo;

enum class ReloadStatus
{
	NONE = -1,
	START,
	END,
};

class Gun : public SpriteGo
{
public:
	enum class Types
	{
		NONE = -1,
		PISTOL,
		RIFLE,
		GUNTYPES
	};
	static constexpr int TotalTypes = (int)Types::GUNTYPES;

	static const int ammoStats[TotalTypes];
	static const int magazineStats[TotalTypes];
	static const float reloadRateStats[TotalTypes];
	static const float fireRateStats[TotalTypes];

	int ammo = 0;
protected:
	Types gunType = Types::NONE;
	ObjectPool<Bullet> poolBullets;

	int maxAmmo = 0;
	int maxMagazine = 0;
	int magazine = 0;

	SoundGo* shootSound;
	SoundGo* shootFailSound;
	SoundGo* reloadSound;
	
	
	ReloadStatus reloadStatus = ReloadStatus::NONE;
	float reloadRate = 0.f;
	float reloadTimer = 0.f;

	float fireRate = 0.f;
	float fireTimer = 0.f;
public:
	Gun(const std::string id = "", const std::string n = "");

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Release() override;
	virtual void Update(float dt)override;

	const int GetAmmo() const;
	const int GetMagazine() const;
	const float GetReloadTimer() const;
	const ReloadStatus GetReloadStatus();

	void SetType(Gun::Types type);
	void SetReloadStatus(ReloadStatus status);
	void SetSound(SoundGo* shootSound, SoundGo* shootFailSound, SoundGo* reloadSound);

	void UpgradeFire();
	void UpgradeClip();
	void Shoot(const sf::Vector2f& position, const sf::Vector2f& look, float dt);
	void Reload();
};