#pragma once
#include "DataTable.h"
#include "Zombie.h"

struct ZombieInfo
{
	std::string textureId;
	float speed;
	int maxHp;
	int damage;
	float attackRate;
};

class ZombieTable : public DataTable
{
protected:
	std::unordered_map<int, ZombieInfo> table;
public:
	ZombieTable() : DataTable(DataTable::Ids::Zombie) {};
	virtual ~ZombieTable() override { Release(); }

	virtual bool Load() override;
	virtual void Release() override;

	const ZombieInfo& Get(int id);
};

