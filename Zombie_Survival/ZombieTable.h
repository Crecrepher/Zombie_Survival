#pragma once
#include "DataTable.h"
#include "Zombie.h"

struct ZombieInfo
{
	Zombie::Types zombieType;
	std::string textureId;
	float speed;
	int maxHp;
	int Damage;
	float attackRate;
};

class ZombieTable : public DataTable
{
protected:
	std::unordered_map<Zombie::Types, ZombieInfo> table;

	Zombie::Types zombieType = Zombie::Types::Bloater;
	std::string textureId ="";
	float speedStat =0.f;
	int hpStat =0;
	int damageStat=0;
	float attackRateStat = 0.f;

public:
	ZombieTable() : DataTable(DataTable::Ids::Zombie) {};

	virtual ~ZombieTable() override = default;

	virtual bool Load() override;
	virtual void Release() override;

	const ZombieInfo& Get(Zombie::Types id);
};

