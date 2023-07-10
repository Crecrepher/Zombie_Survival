#include "stdafx.h"
#include "SceneTitile.h"
#include "SceneMgr.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "SpriteGo.h"
#include "TextGo.h"
#include "SoundGo.h"
#include "Framework.h"
#include <atlstr.h>

#include "DataTableMgr.h"
#include "StringTable.h"

SceneTitile::SceneTitile() : Scene(SceneId::Title)
{
	resources.push_back(std::make_tuple(ResourceTypes::Font, "fonts/neodgm.ttf"));
	resources.push_back(std::make_tuple(ResourceTypes::Font, "fonts/PixelMplus12-Regular.ttf"));
	resources.push_back(std::make_tuple(ResourceTypes::Sound, "sound/win.wav"));
}

SceneTitile::~SceneTitile()
{
}

void SceneTitile::Init()
{
	Release();

	sf::Vector2f windowSize = FRAMEWORK.GetWindowSize();
	sf::Vector2f centerPos = windowSize * 0.5f;

	worldView.setSize(windowSize);
	worldView.setCenter(centerPos);
	uiView.setSize(windowSize);
	uiView.setCenter(centerPos);
	//계속써도되는거 add
	Release();
	AddGo(new SpriteGo("graphics/background.png", "Back"));
	AddGo(new SpriteGo("graphics/icon.png","Icon"));
	AddGo(new TextGo("Text"));
	AddGo(new SoundGo("sound/win.wav","Start"));
	AddGo(new SoundGo("sound/select.wav","MoveSound"));

	for (auto go : gameObjects)
	{
		go->Init();
	}
}

void SceneTitile::Release()
{
	for (auto go : gameObjects)
	{
		//go->Release();
		delete go;
	}

}

void SceneTitile::Enter()
{
	Scene::Enter();
	SpriteGo* findSGo = (SpriteGo*)FindGo("Icon");
	findSGo->SetOrigin(Origins::MC);
	findSGo->SetPosition(FRAMEWORK.GetWindowSize().x / 2.f, FRAMEWORK.GetWindowSize().y / 2.5f);
	findSGo->SetSize(1.5f, 1.5f);
	findSGo->sortLayer = 1;

	findSGo = (SpriteGo*)FindGo("Back");
	findSGo->SetOrigin(Origins::TL);
	findSGo->SetSize(FRAMEWORK.GetWindowSize().x / 1920.f, FRAMEWORK.GetWindowSize().y / 1080.f);
	findSGo->SetPosition(0, 0);

	TextGo* findTGo = (TextGo*)FindGo("Text");
	findTGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/neodgm.ttf"));

	auto stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	std::wstring unicode = L"";
	stringtable->convert_ansi_to_unicode_string(unicode, stringtable->Get("TITLE").c_str(), stringtable->Get("TITLE").size());
	findTGo->text.setString(unicode.c_str());

	findTGo->text.setCharacterSize(75);
	findTGo->text.setFillColor(sf::Color::White);
	Utils::SetOrigin(findTGo->text, Origins::MC);
	findTGo->text.setPosition(FRAMEWORK.GetWindowSize().x * 0.5f, FRAMEWORK.GetWindowSize().y * 0.8f);
	findTGo->sortLayer = 1;

	SoundGo* sou = (SoundGo*)FindGo("Start");
	sou->sound.play();
}

void SceneTitile::Exit()
{
	Scene::Exit();
}

void SceneTitile::Update(float dt)
{
	Scene::Update(dt);	

	TextGo* findTGo = (TextGo*)FindGo("Text");
	findTGo->SetActive(SCENE_MGR.TimeBaseFliper());
	
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Enter))
	{
		SoundGo* sound = (SoundGo*)FindGo("MoveSound");
		sound->sound.play();
		SCENE_MGR.ChangeScene(SceneId::Menu);
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::L))
	{
		if (Variables::CurrntLang == Languages::JP)
		{
			findTGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/PixelMplus12-Regular.ttf"));
		}
		else
		{
			findTGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/neodgm.ttf"));
		}
		auto stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
		std::wstring unicode = L"";
		stringtable->convert_ansi_to_unicode_string(unicode, stringtable->Get("TITLE").c_str(), stringtable->Get("TITLE").size());
		findTGo->text.setString(unicode.c_str()/*L"엔터로 시작"*/);
		Utils::SetOrigin(findTGo->text, Origins::MC);
		
	}
}

void SceneTitile::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}