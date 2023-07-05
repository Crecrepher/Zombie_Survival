#pragma once
#include "Scene.h"
#include "BlockGo.h"
#include "EffectGo.h"
#include <sstream>

class Player;
class VertexArrayGo;
class Zombie;

class SceneDev1 : public Scene
{
protected:

	Player* player;
	std::list<Zombie*> zombiePool;
	std::list<Zombie*> zombies;
	sf::Vector2f tilesize = { 50.f,50.f };

public:
	SceneDev1();
	virtual ~SceneDev1() override;

	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	VertexArrayGo* CreateBackground(sf::Vector2f size, sf::Vector2f tileSize, sf::Vector2f texSize, std::string textureId);

	void CreateZombies(int count);
	void SpawnZombies(int count,sf::Vector2f center, float radius);
	void ClearZombies();

	void OnDieZombie(Zombie* zombie);

	std::list<Zombie*>* GetZombieList();
	sf::Vector2f GetMapTop();
	sf::Vector2f GetMapBot();
};

