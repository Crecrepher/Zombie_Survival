#include "stdafx.h"
#include "Blood.h"
#include "Zombie.h"
#include "SceneMgr.h"
#include "Scene.h"
Blood::Blood(const std::string id, const std::string n)
	:SpriteGo(id,n)
{
}

Blood::~Blood()
{
}

void Blood::Init()
{
	SpriteGo::Init();
	SetOrigin(Origins::MC);
}

void Blood::Reset()
{
	SpriteGo::Reset();
	SetPosition(0.f, 0.f);
	timer = 100;
}

void Blood::Release()
{
	SpriteGo::Release();
}

void Blood::Update(float dt)
{
	SpriteGo::Update(dt);
	timer -= dt*10;
	sprite.setColor(sf::Color::Color(255, 255, 255, timer*2+55));
	if (timer<=0)
	{
		SCENE_MGR.GetCurrScene()->RemoveGo(this);
		pool->Return(this);
		return;
	}
}

void Blood::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}
