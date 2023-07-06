#pragma once
#include "Scene.h"
#include "ObjectPool.h"

#include <sstream>

class Player;
class VertexArrayGo;
class Zombie;
class Blood;

class SceneDev1 : public Scene
{
protected:

	Player* player;
	ObjectPool<Zombie> zombiePool;
	ObjectPool<Blood> bloodPool;

	sf::FloatRect wallBounds;
	bool isGameOver = false;

	int score = 0;
	int hiScore = 0;
	int wave = 0;

	bool pause = true;
	float padeIn = 0;
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

	void SpawnZombies(int count,sf::Vector2f center, float radius);
	void ClearZombies();

	void ClearBloods();

	void OnDieZombie(Zombie* zombie);
	void OnDiePlayer();

	const std::list<Zombie*>* GetZombieList() const;
};

