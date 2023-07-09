#include "stdafx.h"
#include "Gun.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "SceneDev1.h"
#include "Bullet.h"
#include "SoundGo.h"

const int Gun::ammoStats[TotalTypes] = { 300, 0 };
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
	maxMagazine = Gun::magazineStats[(int)gunType];
	reloadRate = Gun::reloadRateStats[(int)gunType];
	fireRate = Gun::fireRateStats[(int)gunType];
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
			magazine = std::min(ammo, maxMagazine);
			ammo -= magazine;
			reloadTimer = 0.f;
			reloadStatus = ReloadStatus::END;
			reloadSound->Play();
		}
	}
}

void Gun::Shoot(const sf::Vector2f& position, const sf::Vector2f& look, float dt)
{
	if (magazine == 0 || fireTimer > 0.f)
	{
		if (INPUT_MGR.GetMouseButtonDown(sf::Mouse::Left))
		{
			shootFailSound->Play();
		}
		return;
	}
	shootSound->Play();
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
	return 1-reloadTimer/reloadRate;
}

void Gun::Reload()
{
	if (magazine == maxMagazine)
	{
		shootFailSound->Play();
		return;
	}
	if (reloadStatus != ReloadStatus::START)
	{
		reloadSound->Play();
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

void Gun::SetSound(SoundGo* shootSound, SoundGo* shootFailSound, SoundGo* reloadSound)
{
	this->shootSound = shootSound;
	this->shootFailSound = shootFailSound;
	this->reloadSound = reloadSound;
}

void Gun::UpgradeFire()
{
	fireRate = std::max(fireRate - 0.02f, 0.05f);
}

void Gun::UpgradeClip()
{
	maxMagazine += 10;
	reloadRate = std::max(reloadRate - 0.2f, 0.f);
}
