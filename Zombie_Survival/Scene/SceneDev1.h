#pragma once
#include "Scene.h"
#include "BlockGo.h"
#include "EffectGo.h"
#include <sstream>

class Player;

class SceneDev1 : public Scene
{
protected:

	Player* player;

public:
	SceneDev1();
	virtual ~SceneDev1() override;

	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
};

