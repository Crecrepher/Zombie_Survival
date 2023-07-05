#include "stdafx.h"
#include "Player.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "SceneDev1.h"
#include "Bullet.h"
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
	SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(SCENE_MGR.GetCurrScene());
	if (sceneDev1 != nullptr)
	{
		mapTop = sceneDev1->GetMapTop();
		mapBot = sceneDev1->GetMapBot();
	}
}

void Player::Reset()
{
	SpriteGo::Reset();
}

void Player::Release()
{
	SpriteGo::Release();
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

	if (position.x < mapTop.x || position.x > mapBot.x )
	{
		direction.x = 0;
	}
	if (position.y < mapTop.y || position.y > mapBot.y)
	{
		direction.y = 0;
	}
	position += direction * speed * dt;

	SetPosition(position);

	//발사
	if (INPUT_MGR.GetMouseButtonDown(sf::Mouse::Left))
	{
		Bullet* bullet = new Bullet("graphics/bullet.png");
		bullet->Init();
		bullet->Reset();
		bullet->Fire(GetPosition(), look, 1000.f);

		Scene* scene = SCENE_MGR.GetCurrScene();
		SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene);
		if (sceneDev1 != nullptr)
		{
			bullet->SetZombieList(sceneDev1->GetZombieList());
			sceneDev1->AddGo(bullet);
		}
	}
}

void Player::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}
