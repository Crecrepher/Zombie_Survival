#include "stdafx.h"
#include "StringTable.h"
#include "rapidcsv.h"
#include <sstream>
#include <string>
#include <iostream>

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

DWORD StringTable::convert_ansi_to_unicode_string(
	__out std::wstring& unicode,
	__in const char* ansi,
	__in const size_t ansi_size
) {

	DWORD error = 0;

	do {

		if ((nullptr == ansi) || (0 == ansi_size)) {
			error = ERROR_INVALID_PARAMETER;
			break;
		}

		unicode.clear();

		//
		// getting required cch.
		//

		int required_cch = ::MultiByteToWideChar(
			CP_ACP,
			0,
			ansi, static_cast<int>(ansi_size),
			nullptr, 0
		);

		if (0 == required_cch) {
			error = ::GetLastError();
			break;
		}

		unicode.resize(required_cch);

		//
		// convert.
		//

		if (0 == ::MultiByteToWideChar(
			CP_ACP,
			0,
			ansi, static_cast<int>(ansi_size),
			const_cast<wchar_t*>(unicode.c_str()), static_cast<int>(unicode.size())
		)) {
			error = ::GetLastError();
			break;
		}

	} while (false);

	return error;
}