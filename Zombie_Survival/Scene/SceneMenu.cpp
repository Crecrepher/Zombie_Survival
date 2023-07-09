#include "stdafx.h"
#include "SceneMenu.h"
#include "SceneMgr.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "EffectGo.h"
#include "RectGo.h"
#include "SoundGo.h"
#include "Framework.h"
#include "TextGo.h"
#include <sstream>
SceneMenu::SceneMenu() : Scene(SceneId::Menu), 
menuIndex(0), closeTimer(0.f), isClose(false), loading(false)
{
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/b1.png"));
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/b2.png"));
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/b3.png"));
	resources.push_back(std::make_tuple(ResourceTypes::Sound, "sound/chop.wav"));
	resources.push_back(std::make_tuple(ResourceTypes::Sound, "sound/death.wav"));
}

SceneMenu::~SceneMenu()
{
}

void SceneMenu::Init()
{
	Release();
	
	sf::Vector2f windowSize = FRAMEWORK.GetWindowSize();
	sf::Vector2f centerPos = windowSize * 0.5f;

	worldView.setSize(windowSize);
	worldView.setCenter(centerPos);
	uiView.setSize(windowSize);
	uiView.setCenter(centerPos);

	AddGo(new SpriteGo("graphics/background.png", "Back"));
	AddGo(new EffectGo("graphics/icon.png","Icon"));
	AddGo(new SpriteGo("graphics/b1.png","Menu1"));
	AddGo(new SpriteGo("graphics/b3.png","Menu3"));
	AddGo(new RectGo("MenuSelector"));
	AddGo(new SoundGo("sound/chop.wav","MoveSound"));
	AddGo(new SoundGo("sound/select.wav","SelectSound"));
	AddGo(new SoundGo("sound/death.wav","ExitSound"));
	AddGo(new TextGo("HiScore"));
	AddGo(new TextGo("Loading"));
	for (auto go : gameObjects)
	{
		go->Init();
	}
}

void SceneMenu::Release()
{
	for (auto go : gameObjects)
	{
		delete go;
	}
}

void SceneMenu::Enter()
{
	Scene::Enter();
	loading = false;
	menuIndex = 0;
	EffectGo* findEGo = (EffectGo*)FindGo("Icon");
	findEGo->SetOrigin(Origins::MC);
	findEGo->SetPosition(FRAMEWORK.GetWindowSize().x / 2.f, FRAMEWORK.GetWindowSize().y / 2.5f);
	findEGo->sortLayer = 1;

	SpriteGo* findSGo = (SpriteGo*)FindGo("Menu3");
	findSGo->SetOrigin(Origins::MC);
	findSGo->SetPosition(FRAMEWORK.GetWindowSize().x *0.75f,
		FRAMEWORK.GetWindowSize().y*0.8f);
	findSGo->SetSize(0.7f, 0.7f);
	findSGo->sortLayer = 3;

	findSGo = (SpriteGo*)FindGo("Back");
	findSGo->SetOrigin(Origins::TL);
	findSGo->SetSize(FRAMEWORK.GetWindowSize().x / 1920.f, FRAMEWORK.GetWindowSize().y / 1080.f);
	findSGo->SetPosition(0, 0);
	findSGo->sortLayer = 0;

	findSGo = (SpriteGo*)FindGo("Menu1");
	findSGo->SetOrigin(Origins::MC);
	findSGo->SetPosition(FRAMEWORK.GetWindowSize().x * 0.25f,
		FRAMEWORK.GetWindowSize().y * 0.8f);
	findSGo->SetSize(0.7f, 0.7f);
	findSGo->sortLayer = 3;

	RectGo* findRGo = (RectGo*)FindGo("MenuSelector");
	findRGo->rectangle.setSize(sf::Vector2f(
		findSGo->GetSize().x* 0.74f,
		findSGo->GetSize().y * 0.74f));
	findRGo->SetOrigin(Origins::MC);
	findRGo->SetPosition(findSGo->GetPosition());
	findRGo->rectangle.setFillColor(sf::Color::White);
	findRGo->sortLayer = 2;

	TextGo* findTGo = (TextGo*)FindGo("HiScore");
	findTGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/zombiecontrol.ttf"));
	std::stringstream ss;
	ss << "HI SCORE:" << hiScore;
	findTGo->text.setString(ss.str());
	findTGo->text.setCharacterSize(60);
	findTGo->text.setFillColor(sf::Color::White);
	Utils::SetOrigin(findTGo->text, Origins::MR);
	findTGo->text.setPosition(FRAMEWORK.GetWindowSize().x - 100.f, 50.f);
	findTGo->sortLayer = 100;

	findTGo = (TextGo*)FindGo("Loading");
	findTGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/zombiecontrol.ttf"));
	findTGo->text.setString("LOADING...");
	findTGo->text.setCharacterSize(200);
	findTGo->text.setFillColor(sf::Color::White);
	Utils::SetOrigin(findTGo->text, Origins::BL);
	findTGo->text.setPosition(50.f, FRAMEWORK.GetWindowSize().y - 50.f);
	findTGo->sortLayer = 102;
	findTGo->SetActive(false);
}

void SceneMenu::Exit()
{
	Scene::Exit();
}

void SceneMenu::Update(float dt)
{
	Scene::Update(dt);

	//로딩화면
	if (loading)
	{
		SCENE_MGR.ChangeScene(SceneId::Game);
	}

	//아이콘 애니메이션
	EffectGo* findGo = (EffectGo*)FindGo("Icon");
	findGo->SetSize(1.3f + (SCENE_MGR.TimerTime() / 10),
		1.3f + (SCENE_MGR.TimerTime() / 10));

	//메뉴선택
	MenuMove();

	//메뉴 선택
	MenuSelect();
	
	if (isClose)
	{
		closeTimer += dt;
	}

	//타이틀로 돌아가기
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Escape))
	{
		SCENE_MGR.ChangeScene(SceneId::Title);
	}
}

void SceneMenu::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
	if (closeTimer >= 2.f)
	{
		window.close();
	}
	window.setMouseCursorVisible(true);
}

void SceneMenu::MenuMove()
{
	RectGo* findRGo = (RectGo*)FindGo("MenuSelector");
	SoundGo* sound = (SoundGo*)FindGo("MoveSound");
	if (menuIndex > 0 && INPUT_MGR.GetKeyDown(sf::Keyboard::Left)) //왼쪽 방향키
	{
		menuIndex--;
		findRGo->SetPosition
		(findRGo->GetPosition().x
			- (FRAMEWORK.GetWindowSize().x * 0.5f),
			findRGo->GetPosition().y);
		sound->sound.play();
	}
	else if (menuIndex < 1 && INPUT_MGR.GetKeyDown(sf::Keyboard::Right)) //오른쪽 방향키
	{
		menuIndex++;
		findRGo->SetPosition
		(findRGo->GetPosition().x
			+ (FRAMEWORK.GetWindowSize().x * 0.5f),
			findRGo->GetPosition().y);
		sound->sound.play();
	}
}

void SceneMenu::MenuSelect()
{
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Return))
	{
		SoundGo* sound = (SoundGo*)FindGo("SelectSound");
		sound->sound.play();
		switch (menuIndex)
		{
		case 0: //1인 플레이
		{
			SpriteGo* findSGo = (SpriteGo*)FindGo("Back");
			findSGo->sortLayer = 101;
			TextGo* findTGo = (TextGo*)FindGo("Loading");
			findTGo->SetActive(true);
			loading = true;
			break;
		}
		case 2: //게임 종료
			EffectGo * icon = (EffectGo*)FindGo("Icon");
			icon->Fire(sf::Vector2f(0.f, -1000.f));
			sound = (SoundGo*)FindGo("ExitSound");
			sound->sound.play();
			isClose = true;
			break;
		}
	}
}
