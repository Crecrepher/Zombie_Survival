#pragma once
#include "SpriteGo.h"
#include "ObjectPool.h"

class Blood : public SpriteGo
{
protected:
	float timer = 100;
public:
	ObjectPool<Blood> pool;
	Blood(const std::string id = "", const std::string n = "");
	virtual ~Blood() override;

	virtual void Init()override;
	virtual void Reset()override;
	virtual void Release() override;

	virtual void Update(float dt)override;
	virtual void Draw(sf::RenderWindow& window)override;
};

