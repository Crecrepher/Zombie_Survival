#include "stdafx.h"
#include "StringTable.h"
#include "rapidcsv.h"

bool StringTable::Load()
{
	std::vector< std::string >filenames;
	filenames.push_back("tables/StringTable_kor.csv");
	filenames.push_back("tables/StringTable_eng.csv");
	filenames.push_back("tables/StringTable_jap.csv");

	for (int i = 0; i <tables.size(); ++i)
	{
		rapidcsv::Document doc(filenames[i]);
		std::vector<std::string> ids = doc.GetColumn<std::string>(0);
		std::vector<std::string> values = doc.GetColumn<std::string>(1);
		for (int j = 0; j < ids.size(); ++j)
		{
			/*std::cout << ids[i] << " " << values[i] << std::endl;*/
			tables[i].insert({ids[j],values[j]});
		}
	}


	return true;
}

void StringTable::Release()
{
	for (auto table : tables)
	{
		table.clear();
	}
	//tables.clear();
}

const std::string& StringTable::Get(const std::string& id, Languages lang) const
{
	auto& table = tables[(int)lang];

	auto find = table.find(id);
	if (find == table.end())
	{
		std::cout << "ERR: Undefined" << std::endl;
		return "ERR: Undefined";
	}

	return find->second;
}

std::wstring& StringTable::GetW(const std::string& id, Languages lang) const
{
	auto& table = tables[(int)lang];

	auto find = table.find(id);
	if (find == table.end())
	{
		std::cout << "ERR: Undefined" << std::endl;
		std::wstring errors = L"ERR: Undefined";
		return errors;
	}
	std::cout << find->second << std::endl;
	std::wstring wstr(find->second.begin(), find->second.end()); // std::string을 std::wstring으로 변환
	return wstr;
}
