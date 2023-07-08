#include "stdafx.h"
#include "Player.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "SceneDev1.h"
#include "Zombie.h"
#include "Gun.h"

#include <math.h>
Player::Player(const std::string id,const std::string n) :SpriteGo(id,n), speed(300.f)
{
	AddGun(Gun::Types::PISTOL);
	AddGun(Gun::Types::RIFLE);
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
	
	for (auto gun : gunArray)
	{
		gun->Init();
	}
}

void Player::Reset()
{
	SpriteGo::Reset();

	hp = maxHp;
	isAlive = true;

	for (auto gun : gunArray)
	{
		gun->Reset();
	}
}

void Player::Release()
{
	SpriteGo::Release();

	for (auto gun : gunArray)
	{
		gun->Release();
	}
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


	//발사
	if (INPUT_MGR.GetMouseButtonDown(sf::Mouse::Left) || INPUT_MGR.GetMouseButton(sf::Mouse::Button::Left))
	{
		gunArray[currentGunIndex]->Shoot(GetPosition(), look, dt);
	}
	
	//피격 무적시간
	if (invincibility > 0)
	{
		invincibility -= (dt * 100);
	}
	else
	{
		sprite.setColor(sf::Color::White);
	}

	//재장전
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::R))
	{
		gunArray[currentGunIndex]->Reload();
	}

	//무기 변경
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Q))
	{
		currentGunIndex = 0;
	}
	else if (INPUT_MGR.GetKeyDown(sf::Keyboard::E))
	{
		currentGunIndex = 1;
	}

	gunArray[currentGunIndex]->Update(dt);
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

std::stringstream Player::GetAmmoInfo()
{
	return std::stringstream() << gunArray[currentGunIndex]->GetMagazine() << " / " << gunArray[currentGunIndex]->GetAmmo();
}

void Player::AddGun(Gun::Types type)
{
	Gun* gun = new Gun;
	gun->SetType(type);
	gunArray.push_back(gun);
}

const ReloadStatus Player::GetReloadStatus() const
{
	return gunArray[currentGunIndex]->GetReloadStatus();
}

void Player::SetReloadStatus(ReloadStatus status)
{
	gunArray[currentGunIndex]->SetReloadStatus(status);
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
