#include "stdafx.h"
#include "Player.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "SceneDev1.h"
#include "Zombie.h"

#include <math.h>
Player::Player(const std::string id,const std::string n) :SpriteGo(id,n), speed(300.f)
{
}

void Player::SetPosition(float x, float y)
{
	SpriteGo::SetPosition(x, y);
}

void Player::SetPosition(const sf::Vector2f& p)
{
	SpriteGo::SetPosition(p);
}

void Player::Init()
{
	SpriteGo::Init();
	SetOrigin(Origins::MC);

	ObjectPool<Bullet>* ptr = &poolBullets;
	poolBullets.OnCreate = [ptr](Bullet* bullet) {
		bullet->textureId = "graphics/bullet.png";
		bullet->pool = ptr;
	};
	poolBullets.Init();
}

void Player::Reset()
{
	SpriteGo::Reset();

	hp = maxHp;
	isAlive = true;

	for (auto bullet : poolBullets.GetUseList())
	{
		SCENE_MGR.GetCurrScene()->RemoveGo(bullet);
	}
	poolBullets.Clear();
	ammo = maxAmmo;
	magazine = maxMagazine;
	reloadRateTimer = 0.f;
}

void Player::Release()
{
	SpriteGo::Release();

	poolBullets.Release();
}

void Player::Update(float dt)
{
	SpriteGo::Update(dt);

	sf::Vector2f mousePos = INPUT_MGR.GetMousePos();
	sf::Vector2f mouseWorldPos = SCENE_MGR.GetCurrScene()->ScreenToWorldPos(mousePos);
	sf::Vector2f playerScreenPos = SCENE_MGR.GetCurrScene()->worldPosToScreen(position);

	//ȸ��
	look = Utils::Normalize(mousePos - playerScreenPos);
	sprite.setRotation( Utils::Angle(look));

	//�̵�
	direction.x = INPUT_MGR.GetAxis(Axis::Horizontal);
	direction.y = INPUT_MGR.GetAxis(Axis::Vertical);

	float magnitude = Utils::Magnitude(direction);
	if (magnitude > 1.f)
	{
		direction /= magnitude;
	}

	position += direction * speed * dt;

	if (!wallBounds.contains(position))
	{
		position = Utils::Clamp(position,wallBoundsLT,wallBoundsRB);
	}
	SetPosition(position);


	//�߻�
	if (INPUT_MGR.GetMouseButtonDown(sf::Mouse::Left) && magazine > 0)
	{
		Bullet* bullet = poolBullets.Get();
		bullet->Fire(GetPosition(), look, 1000.f);
		magazine--;
		Scene* scene = SCENE_MGR.GetCurrScene();
		SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene);
		if (sceneDev1 != nullptr)
		{
			bullet->SetZombieList(sceneDev1->GetZombieList());
			sceneDev1->AddGo(bullet);
		}
	}
	if (invincibility > 0)
	{
		invincibility -= (dt * 100);
	}
	else
	{
		sprite.setColor(sf::Color::White);
	}

	//������
	bool isReload = (reloadStatus == ReloadStatus::START) ? true : false;
	bool reloadPossible = INPUT_MGR.GetKeyDown(sf::Keyboard::R) && magazine < maxMagazine;
	if (!isReload && (reloadPossible || magazine == 0))
	{
		ammo += magazine;
		magazine = 0;
		reloadStatus = ReloadStatus::START;
	}
	if (isReload)
	{
		reloadRateTimer += dt;
		if (reloadRateTimer >= reloadRate)
		{
			ammo -= maxMagazine;
			magazine = maxMagazine;
			reloadRateTimer = 0.f;
			reloadStatus = ReloadStatus::END;
		}
	}
}

void Player::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}

void Player::SetWallBounds(const sf::FloatRect& bounds)
{
	wallBounds = bounds;

	wallBoundsLT = { wallBounds.left,wallBounds.top };
	wallBoundsRB = { wallBounds.left + wallBounds.width,wallBounds.top + wallBounds.height };
}

void Player::Ouch(float dt)
{
	if (invincibility <=0)
	{
		hp -= 5;
		invincibility = 25;
		sprite.setColor(sf::Color::Color(205, 12, 34));
	}
}

int Player::GetHp()
{
	return hp;
}

const int Player::GetAmmo() const
{
	return ammo;
}

const int Player::GetMagazine() const
{
	return magazine;
}

const ReloadStatus Player::GetReload() const
{
	return reloadStatus;
}

void Player::SetReloadStatus(ReloadStatus status)
{
	reloadStatus = status;
}

void Player::OnHitted(int damdge)
{
	if (!isAlive)
	{
		return;
	}

	hp = std::max(hp - damdge,0);
	sprite.setColor(sf::Color::Color(205, 12, 34,200));
	invincibility = 25;
	if (hp == 0)
	{
		OnDie();
	}
}

void Player::OnDie()
{
	isAlive = false;

	SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(SCENE_MGR.GetCurrScene());
	if (sceneDev1 != nullptr)
	{
		sceneDev1->OnDiePlayer();
	}
}
