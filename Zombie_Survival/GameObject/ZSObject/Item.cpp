#include "stdafx.h"
#include "Item.h"
#include "Player.h"

Item::Item(const std::string id, const std::string n)
	:SpriteGo(id,n)
{
}

void Item::Init()
{
	SpriteGo::Init();
	SetOrigin(Origins::MC);
	SetActive(false);
}

void Item::Reset()
{
	SpriteGo::Reset();
}

void Item::Update(float dt)
{
	SpriteGo::Update(dt);
	timer -= dt;
	if (player->sprite.getGlobalBounds().intersects(sprite.getGlobalBounds()))
	{
		switch (itemType)
		{
		case Item::Types::Hp:
			player->ItemHealEat(ability);
			break;
		case Item::Types::Ammo:
			player->ItemAmmoEat(10+ability*2);
			break;
		}
		isSpawn = false;
		SetActive(false);
	}
	if (timer<=0)
	{
		isSpawn = false;
		SetActive(false);
	}
}

void Item::TryMake(sf::Vector2f pos)
{
	if (/*Utils::RandomRange(luck, 1.f)*/ 1>= 0.95f)
	{
		SetPosition(pos);
		isSpawn = true;
		SetActive(true);
		timer = maxTime;
	}

}
