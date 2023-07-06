#pragma once
#include "SpriteGo.h"
#include "ObjectPool.h"
class Zombie;


class Bullet : public SpriteGo
{
protected:
	sf::Vector2f direction;
	float speed = 0.f;
	float range = 2000.f;
	int damage = 25;

	const std::list<Zombie*>* zombies;

	
public:
	ObjectPool<Bullet>* pool;
	Bullet(const std::string id = "", const std::string n = "");
	virtual ~Bullet() override;

	void SetZombieList(const std::list<Zombie*>* list);
	void Fire(const sf::Vector2f& pos, const sf::Vector2f& dir, float speed);

	virtual void Init()override;
	virtual void Reset()override;
	virtual void Release() override;

	virtual void Update(float dt)override;
	virtual void Draw(sf::RenderWindow& window)override;
};

