#pragma once
#include "SpriteGo.h"

class Player;
class SoundGo;

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
	float maxTime = 15.f;

	Player* player = nullptr;
	SoundGo* eatSound = nullptr;
	SoundGo* dropSound = nullptr;
public:
	Item(const std::string id = "", const std::string n = "");
	virtual ~Item() override {};

	virtual void Init()override;
	virtual void Reset()override;
	virtual void Update(float dt)override;

	void SetPlayer(Player* player) {this->player = player;}
	void SetSound(SoundGo* eatSound, SoundGo* dropSound);
	void SetType(Types type);
	bool GetSpawn() const { return isSpawn; }
	void TryMake(sf::Vector2f pos);
	void ItemUpgrade();

	
};

