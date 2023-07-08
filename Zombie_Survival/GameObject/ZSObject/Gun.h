#pragma once
#include "SpriteGo.h"
#include "ObjectPool.h"

class Bullet;

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
	};
	static constexpr int TotalTypes = 2;

	static const int ammoStats[TotalTypes];
	static const int magazineStats[TotalTypes];
	static const float reloadRateStats[TotalTypes];
	static const float fireRateStats[TotalTypes];

protected:
	Types gunType = Types::NONE;
	ObjectPool<Bullet> poolBullets;

	int maxAmmo = 0;
	int ammo = 0;

	int maxMagazine = 0;
	int magazine = 0;
	
	ReloadStatus reloadStatus = ReloadStatus::NONE;
	float reloadRate = 0.f;
	float reloadRateTimer = 0.f;

	float fireRate = 0.f;
	float fireRateTimer = 0.f;
public:
	Gun(const std::string id = "", const std::string n = "");

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Release() override;
	virtual void Update(float dt)override;

	const int GetAmmo() const;
	const int GetMagazine() const;
	const ReloadStatus GetReloadStatus();

	void SetType(Gun::Types type);
	void SetReloadStatus(ReloadStatus status);

	void Shoot(const sf::Vector2f& position, const sf::Vector2f& look, float dt);
	void Reload();
};