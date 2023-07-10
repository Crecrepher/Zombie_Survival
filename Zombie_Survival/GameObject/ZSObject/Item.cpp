#include "stdafx.h"
#include "Item.h"
#include "Player.h"
#include "SceneGame.h"
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
			player->ItemAmmoEat(20+ability*2);
			SceneGame* sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrScene());
			if (sceneGame != nullptr)
			{
				sceneGame->AmmoUiUpdate();
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
	if (Utils::RandomRange(luck, 1.f)>= 0.90f)
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
