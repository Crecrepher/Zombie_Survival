#include "stdafx.h"
#include "Bullet.h"
#include "Zombie.h"
#include "SceneMgr.h"
#include "Scene.h"

const int Bullet::damageStats[Gun::TotalTypes] = { 25, 25 };
const float Bullet::rangeStats[Gun::TotalTypes] = { 400.f, 800.f };

Bullet::Bullet(const std::string id, const std::string n)
	:SpriteGo(id,n)
{
}

Bullet::~Bullet()
{
}

void Bullet::SetZombieList(const std::list<Zombie*>* list)
{
	zombies = list;
}

void Bullet::Fire(const sf::Vector2f& pos, const sf::Vector2f& dir, float speed)
{
	sprite.setRotation(Utils::Angle(dir));
	SetPosition(pos);
	direction = dir;
	this->speed = speed;
}

void Bullet::Init()
{
	SpriteGo::Init();
	SetOrigin(Origins::MC);
	damage = Bullet::damageStats[(int)*gunType];
	range = Bullet::rangeStats[(int)*gunType];
}

void Bullet::Reset()
{
	SpriteGo::Reset();

	sprite.setRotation(0.f);
	SetPosition(0.f, 0.f);
	direction = { 0.f, 0.f };
	speed = 0.f;
	range = Bullet::rangeStats[(int)*gunType];
}

void Bullet::Release()
{
	SpriteGo::Release();
}

void Bullet::Update(float dt)
{
	SpriteGo::Update(dt);

	range -= speed * dt;
	if (range <= 0.f)
	{
		SCENE_MGR.GetCurrScene()->RemoveGo(this);
		pool->Return(this);
		return;
	}
	
	position += direction * speed * dt;
	sprite.setPosition(position);

	if (zombies != nullptr)
	{
		for (Zombie* zombie : *zombies)
		{
			if (sprite.getGlobalBounds().intersects(zombie->sprite.getGlobalBounds()))
			{
				zombie->OnHitBullet(damage);
				SCENE_MGR.GetCurrScene()->RemoveGo(this);
				pool->Return(this);
				break;
			}
		}
	}

}

void Bullet::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}

void Bullet::SetGunType(const Gun::Types* gunType)
{
	this->gunType = gunType;
}
