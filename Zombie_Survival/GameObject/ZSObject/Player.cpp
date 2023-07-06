#include "stdafx.h"
#include "Player.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "SceneDev1.h"
#include "Zombie.h"

#include <math.h>
Player::Player(const std::string id,const std::string n) :SpriteGo(id,n), speed(200.f)
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

	//회전
	look = Utils::Normalize(mousePos - playerScreenPos);
	sprite.setRotation( Utils::Angle(look));

	//이동
	direction.x = INPUT_MGR.GetAxisRaw(Axis::Horizontal);
	direction.y = INPUT_MGR.GetAxisRaw(Axis::Vertical);
	position += direction * speed * dt;
	if (!wallBounds.contains(position))
	{
		position = Utils::Clamp(position,wallBoundsLT,wallBoundsRB);
	}
	SetPosition(position);


	//발사
	if (INPUT_MGR.GetMouseButtonDown(sf::Mouse::Left)&&ammo>0)
	{
		Bullet* bullet = poolBullets.Get();
		bullet->Fire(GetPosition(), look, 1000.f);
		ammo--;
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
