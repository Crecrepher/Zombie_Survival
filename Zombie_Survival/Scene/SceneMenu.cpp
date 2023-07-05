#include "stdafx.h"
#include "SceneMenu.h"
#include "SceneMgr.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "EffectGo.h"
#include "RectGo.h"
#include "SoundGo.h"
#include "Framework.h"


SceneMenu::SceneMenu() : Scene(SceneId::Menu), 
menuIndex(0), closeTimer(0.f), isClose(false)
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

	AddGo(new EffectGo("graphics/icon.png","Icon"));
	AddGo(new SpriteGo("graphics/b1.png","Menu1"));
	AddGo(new SpriteGo("graphics/b2.png","Menu2"));
	AddGo(new SpriteGo("graphics/b3.png","Menu3"));
	AddGo(new RectGo("MenuSelector"));
	AddGo(new SoundGo("MoveSound"));
	AddGo(new SoundGo("SelectSound"));
	AddGo(new SoundGo("ExitSound"));
	for (auto go : gameObjects)
	{
		go->Init();
	}
}

void SceneMenu::Release()
{
	for (auto go : gameObjects)
	{
		//go->Release();
		delete go;
	}
	//RESOURCE_MGR.GetSoundBuffer
}

void SceneMenu::Enter()
{
	menuIndex = 0;
	Scene::Enter();
	EffectGo* findEGo = (EffectGo*)FindGo("Icon");
	findEGo->SetOrigin(Origins::MC);
	findEGo->SetPosition(FRAMEWORK.GetWindowSize().x / 2.f, FRAMEWORK.GetWindowSize().y / 3.f);
	findEGo->sortLayer = 1;

	SpriteGo* findSGo = (SpriteGo*)FindGo("Menu3");
	findSGo->SetOrigin(Origins::MC);
	findSGo->SetPosition(FRAMEWORK.GetWindowSize().x *0.75f,
		FRAMEWORK.GetWindowSize().y*0.8f);
	findSGo->SetSize(0.7f, 0.7f);
	findSGo->sortLayer = 3;

	findSGo = (SpriteGo*)FindGo("Menu2");
	findSGo->SetOrigin(Origins::MC);
	findSGo->SetPosition(FRAMEWORK.GetWindowSize().x * 0.5f,
		FRAMEWORK.GetWindowSize().y * 0.8f);
	findSGo->SetSize(0.7f, 0.7f);
	findSGo->sortLayer = 3;

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

	SoundGo* soundGo = (SoundGo*)FindGo("MoveSound");
	soundGo->sound.setBuffer(*RESOURCE_MGR.GetSoundBuffer("sound/chop.wav"));
	
	soundGo = (SoundGo*)FindGo("SelectSound");
	soundGo->sound.setBuffer(*RESOURCE_MGR.GetSoundBuffer("sound/select.wav"));

	soundGo = (SoundGo*)FindGo("ExitSound");
	soundGo->sound.setBuffer(*RESOURCE_MGR.GetSoundBuffer("sound/death.wav"));
}

void SceneMenu::Exit()
{
	Scene::Exit();
}

void SceneMenu::Update(float dt)
{
	Scene::Update(dt);

	//������ �ִϸ��̼�
	EffectGo* findGo = (EffectGo*)FindGo("Icon");
	findGo->SetSize(1.3 + (SCENE_MGR.TimerTime() / 10),
		1.3 + (SCENE_MGR.TimerTime() / 10));

	//�޴�����
	RectGo* findRGo = (RectGo*)FindGo("MenuSelector");
	SoundGo* sound = (SoundGo*)FindGo("MoveSound");
	if (menuIndex > 0 && INPUT_MGR.GetKeyDown(sf::Keyboard::Left)) //���� ����Ű
	{
		menuIndex--;
		findRGo->SetPosition
		(findRGo->GetPosition().x 
			- (FRAMEWORK.GetWindowSize().x * 0.25f),
			findRGo->GetPosition().y);
		sound->sound.play();
	}
	else if (menuIndex < 2 && INPUT_MGR.GetKeyDown(sf::Keyboard::Right)) //������ ����Ű
	{
		menuIndex++;
		findRGo->SetPosition
		(findRGo->GetPosition().x 
			+ (FRAMEWORK.GetWindowSize().x * 0.25f),
			findRGo->GetPosition().y);
		sound->sound.play();
	}

	//�޴� ����
	EffectGo* icon = (EffectGo*)FindGo("Icon");
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Return))
	{
		sound = (SoundGo*)FindGo("SelectSound");
		sound->sound.play();
		switch (menuIndex)
		{
		case 0: //1�� �÷���
			SCENE_MGR.ChangeScene(SceneId::Game);
			SCENE_MGR.BgmOn();
			break;

		case 1: //2�� �÷���
			SCENE_MGR.ChangeScene(SceneId::Dev1);
			SCENE_MGR.BgmOn();
			break;
		case 2: //���� ����
			sound = (SoundGo*)FindGo("ExitSound");
			icon->Fire(sf::Vector2f(0.f, -1000.f));
			sound->sound.play();
			isClose = true;
			break;
		}
	}
	if (isClose)
	{
		closeTimer += dt;
	}

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Escape))
	{
		SCENE_MGR.ChangeScene(SceneId::Title);
		sound->sound.play();
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
