#include "stdafx.h"
#include "Zombie.h"
#include "InputMgr.h"
#include "Player.h"
#include "SceneMgr.h"
#include "SceneGame.h"
#include "ResourceMgr.h"
#include "SoundGo.h"
#include "DataTableMgr.h"
#include "ZombieTable.h"

//특징별 성능을 블록처리하듯 모듈러 가능한 디자인을 하기

Zombie::Zombie(const std::string n)
	:SpriteGo("", n)
{
}

Zombie::~Zombie()
{
}

void Zombie::Init()
{
	SpriteGo::Init();
	SetOrigin(Origins::MC);
}

void Zombie::Reset()
{
	SpriteGo::Reset();
	hp = maxHp;
	attackTimer = attackRate;
	crawlDash = 0.f;
}

void Zombie::Release()
{
	SpriteGo::Release();
}

void Zombie::Update(float dt)
{
	SpriteGo::Update(dt);
	if (player == nullptr)
	{
		return;
	}

	float distance = Utils::Distance(player->GetPosition(), position);

	look = direction = Utils::Normalize(player->GetPosition() - position);
	sprite.setRotation(Utils::Angle(look));

	if (crawlDash > 0.f)
	{
		crawlDash -= dt;
		position += direction * crawlDash*1000.f * dt;
		SetPosition(position);
	}
	else if (distance >25.f)
	{
		position += direction * speed * dt;
		SetPosition(position);
	}
	attackTimer += dt;
	if (attackTimer > attackRate)
	{	
		if (player->isAlive && sprite.getGlobalBounds().intersects(player->sprite.getGlobalBounds()))
		{
			player->OnHitted(damage);
			attackTimer = 0.f; 
			if (zombieType == Types::Bloater)
			{
				player->OnHittedByBloat(Utils::Normalize(player->GetPosition()- GetPosition()));
			}
		}
	}
	if (zombieType == Types::Chaser)
	{
		speed += dt;
	}
}

void Zombie::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}

void Zombie::SetType(Types t)
{
	zombieType = t;
	const ZombieInfo& info = DATATABLE_MGR.Get<ZombieTable>(DataTable::Ids::Zombie)->Get((int)t);

	int index = (int)zombieType;
	textureId = info.textureId;
	speed = info.speed;
	maxHp = info.maxHp;
	damage = info.damage;
	attackRate = info.attackRate;
}

Zombie::Types Zombie::GetType() const
{
	return zombieType;
}

void Zombie::SetPlayer(Player* player)
{
	this->player = player;
}

void Zombie::OnHitBullet(int damage)
{
	hp -= damage;
	hitedSound->sound.play();
	if (hp<=0)
	{
		Scene* scene = SCENE_MGR.GetCurrScene(); //형변환연산자 쓰기
		SceneGame* sceneGame = dynamic_cast<SceneGame*>(scene);
		if (sceneGame != nullptr)
		{
			sceneGame->OnDieZombie(this);
		}
	}
	if (zombieType == Types::Crawler)
	{
		crawlDash = 0.5;
	}
}
