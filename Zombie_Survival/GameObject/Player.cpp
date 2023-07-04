#include "stdafx.h"
#include "Player.h"
#include "InputMgr.h"

#include <math.h>
Player::Player(const std::string id,const std::string n) :SpriteGo(id,n), speed(1000.f)
{
}

void Player::Init()
{
	SpriteGo::Init();
	SetOrigin(Origins::MC);
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

	//회전
	look = Utils::Normalize(mousePos - position);
	sprite.setRotation( Utils::Angle(look));

	//이동
	direction.x = INPUT_MGR.GetAxisRaw(Axis::Horizontal);
	direction.y = INPUT_MGR.GetAxisRaw(Axis::Vertical);
	position += direction * speed * dt;
	SetPosition(position);
}

void Player::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}
