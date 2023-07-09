#include "stdafx.h"
#include "Gun.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "SceneDev1.h"
#include "Bullet.h"

const int Gun::ammoStats[TotalTypes] = { 300, 900 };
const int Gun::magazineStats[TotalTypes] = { 15, 30 };
const float Gun::reloadRateStats[TotalTypes] = { 2.0f, 3.0f };
const float Gun::fireRateStats[TotalTypes] = { 0.3f, 0.1f };

Gun::Gun(const std::string id, const std::string n)
	:SpriteGo(id, n)
{

}

void Gun::Init()
{
	SpriteGo::Init();
	maxAmmo = Gun::ammoStats[(int)gunType];
	maxMagazine = Gun::magazineStats[(int)gunType];
	reloadRate = Gun::reloadRateStats[(int)gunType];
	fireRate = Gun::fireRateStats[(int)gunType];

	ObjectPool<Bullet>* ptr = &poolBullets;
	poolBullets.OnCreate = [ptr, this](Bullet* bullet) {
		bullet->textureId = "graphics/bullet.png";
		bullet->SetGunType(&gunType);
		bullet->pool = ptr;
	};
	poolBullets.Init();
}

void Gun::Reset()
{
	SpriteGo::Reset();
	for (auto bullet : poolBullets.GetUseList())
	{
		SCENE_MGR.GetCurrScene()->RemoveGo(bullet);
	}
	ammo = maxAmmo;
	magazine = maxMagazine;
	reloadTimer = 0.f;
	poolBullets.Clear();
}

void Gun::Release()
{
	SpriteGo::Release();
	poolBullets.Release();
}

void Gun::Update(float dt)
{
	SpriteGo::Update(dt);
	if (fireTimer > 0.f)
	{
		fireTimer -= dt;
	}

	if (reloadStatus == ReloadStatus::START)
	{
		reloadTimer += dt;
		if (reloadTimer >= reloadRate)
		{
			ammo -= maxMagazine;
			magazine = maxMagazine;
			reloadTimer = 0.f;
			reloadStatus = ReloadStatus::END;
		}
	}
}

void Gun::Shoot(const sf::Vector2f& position, const sf::Vector2f& look, float dt)
{
	if (magazine == 0 || fireTimer > 0.f)
	{
		return;
	}
	fireTimer = fireRate;

	Bullet* bullet = poolBullets.Get();
	bullet->Fire(position, look, 1000.f);
	magazine--;

	Scene* scene = SCENE_MGR.GetCurrScene();
	SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene);
	if (sceneDev1 != nullptr)
	{
		bullet->SetZombieList(sceneDev1->GetZombieList());
		sceneDev1->AddGo(bullet);
	}

	if (magazine == 0)
	{
		Reload();
	}
}

const int Gun::GetAmmo() const
{
	return ammo;
}

const int Gun::GetMagazine() const
{
	return magazine;
}

const float Gun::GetReloadTimer() const
{
	return reloadTimer;
}

void Gun::Reload()
{
	if (magazine == maxMagazine)
	{
		return;
	}
	if (reloadStatus != ReloadStatus::START)
	{
		ammo += magazine;
		magazine = 0;
		reloadStatus = ReloadStatus::START;
	}
}

const ReloadStatus Gun::GetReloadStatus()
{
	return reloadStatus;
}

void Gun::SetType(Gun::Types type)
{
	gunType = type;
}

void Gun::SetReloadStatus(ReloadStatus status)
{
	reloadStatus = status;
}
