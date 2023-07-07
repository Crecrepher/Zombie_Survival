#pragma once
#include "SpriteGo.h"

class Player;

class Item : public SpriteGo
{
public:
	enum class Types
	{
		Hp,
		Ammo,
		TypeCount
	};
private:
	Types itemType;
	int ability = 5;
	float luck = 0.0;
	bool isSpawn = false;
	float timer = 0.f;
	float maxTime = 30.f;

	Player* player = nullptr;
public:
	Item(const std::string id = "", const std::string n = "");
	virtual ~Item() override {};

	virtual void Init()override;
	virtual void Reset()override;
	virtual void Update(float dt)override;

	void SetPlayer(Player* player) {this->player = player;}
	void SetType(Types type) { itemType = type; }
	bool GetSpawn() const { return isSpawn; }
	void TryMake(sf::Vector2f pos);
};

