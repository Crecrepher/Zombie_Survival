#include "stdafx.h"

DWORD convert_ansi_to_unicode_string(std::wstring& unicode, const char* ansi, const size_t ansi_size);

DWORD convert_ansi_to_unicode_string(
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

//example - convert_ansi_to_unicode_string함수가 StringTable에 있음
/*
	TextGo* findTGo = (TextGo*)FindGo("Text");
	findTGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/neodgm.ttf"));

	//*******************************************************************************
	auto stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	std::wstring unicode = L"";
	stringtable->convert_ansi_to_unicode_string(unicode, stringtable->Get("TITLE").c_str(), stringtable->Get("TITLE").size());
	findTGo->text.setString(unicode.c_str());
	//*******************************************************************************

	findTGo->text.setCharacterSize(125);
	findTGo->text.setFillColor(sf::Color::White);
	Utils::SetOrigin(findTGo->text, Origins::MC);
	findTGo->text.setPosition(FRAMEWORK.GetWindowSize().x * 0.5f, FRAMEWORK.GetWindowSize().y * 0.7f);
	findTGo->sortLayer = 1;
*/