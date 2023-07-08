#include "stdafx.h"
#include "Item.h"
#include "Player.h"
#include "SceneDev1.h"
#include "SceneMgr.h"
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
	luck = 0.0;
	ability = 5;
}

void Item::Update(float dt)
{
	SpriteGo::Update(dt);
	timer -= dt;
	if (player->sprite.getGlobalBounds().intersects(sprite.getGlobalBounds()))
	{
		eatSound->sound.play();
		switch (itemType)
		{
		case Item::Types::Hp:
			player->ItemHealEat(ability);
			break;
		case Item::Types::Ammo:
			player->ItemAmmoEat(10+ability*2);
			SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(SCENE_MGR.GetCurrScene());
			if (sceneDev1 != nullptr)
			{
				sceneDev1->AmmoUiUpdate();
			}
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

void Item::SetSound(SoundGo* eatSound, SoundGo* dropSound)
{
	this->eatSound = eatSound; 
	this->dropSound = dropSound;
}

void Item::SetType(Types type)
{ 
	itemType = type;
	if (type ==Types::Ammo)
	{
		luck += 0.1f;
	}
}

void Item::TryMake(sf::Vector2f pos)
{
	if (Utils::RandomRange(luck, 1.f)>= 0.95f)
	{
		dropSound->sound.play();
		SetPosition(pos);
		isSpawn = true;
		SetActive(true);
		timer = maxTime;
	}
}

void Item::ItemUpgrade() 
{
	if (luck < 0.8f)
	{
		luck += 0.05;
	}
	ability += 5;
}
