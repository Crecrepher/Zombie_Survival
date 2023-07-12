#include "stdafx.h"
#include "ZombieTable.h"
#include "rapidcsv.h"

bool ZombieTable::Load()
{
	rapidcsv::Document doc("tables/ZombieTable.csv");
	std::vector<int> ids = doc.GetColumn<int>(0);
	std::vector<std::string> texture = doc.GetColumn<std::string>(1);
	std::vector<float> speed = doc.GetColumn<float>(2);
	std::vector<int> hp = doc.GetColumn<int>(3);
	std::vector<int> damage = doc.GetColumn<int>(4);
	std::vector<float> attackRate = doc.GetColumn<float>(5);
	for (int i = 0; i < ids.size(); ++i)
	{
		table.insert({ ids[i],{texture[i],speed[i],hp[i],damage[i],attackRate[i]}});
	}

	return true;
}

void ZombieTable::Release()
{
	table.clear();
}

const ZombieInfo& ZombieTable::Get(int id)
{
	auto& ztable = table;

	auto find = table.find(id);
	if (find == table.end())
	{
		std::cout << "ERR: Undefined" << std::endl;
		return { "", 0.f, 0, 0, 0.f };
	}
	//예외 throw / 포인터로 넘겨주기

	return find->second;
}
