#include "stdafx.h"
#include "SceneTitile.h"
#include "SceneMgr.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "SpriteGo.h"
#include "TextGo.h"
#include "SoundGo.h"
#include "Framework.h"

SceneTitile::SceneTitile() : Scene(SceneId::Title)
{
	resources.push_back(std::make_tuple(ResourceTypes::Font, "fonts/neodgm.ttf"));
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
	//��ӽᵵ�Ǵ°� add
	Release();
	AddGo(new SpriteGo("graphics/background.png", "Back"));
	AddGo(new SpriteGo("graphics/icon.png","Icon"));
	AddGo(new TextGo("Text"));
	AddGo(new SoundGo("Start"));
	AddGo(new SoundGo("MoveSound"));

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
	findSGo->sortLayer = 1;

	findSGo = (SpriteGo*)FindGo("Back");
	findSGo->SetOrigin(Origins::TL);
	findSGo->SetSize(0.7, 0.7);
	findSGo->SetPosition(0, 0);

	TextGo* findTGo = (TextGo*)FindGo("Text");
	findTGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/neodgm.ttf"));
	findTGo->text.setString("PRESS ENTER");
	findTGo->text.setCharacterSize(125);
	findTGo->text.setFillColor(sf::Color::White);
	Utils::SetOrigin(findTGo->text, Origins::MC);
	findTGo->text.setPosition(FRAMEWORK.GetWindowSize().x * 0.5f, FRAMEWORK.GetWindowSize().y * 0.7f);
	findTGo->sortLayer = 1;

	SoundGo* sou = (SoundGo*)FindGo("Start");
	sou->sound.setBuffer(*RESOURCE_MGR.GetSoundBuffer("sound/win.wav"));
	sou->sound.play();
	sou = (SoundGo*)FindGo("MoveSound");
	sou->sound.setBuffer(*RESOURCE_MGR.GetSoundBuffer("sound/select.wav"));
}

void SceneTitile::Exit()
{
	Scene::Exit();
}

void SceneTitile::Update(float dt)
{
	Scene::Update(dt);

	SpriteGo* findGo = (SpriteGo*)FindGo("Icon");
	findGo->SetSize(1.5f, 1.5f);

	TextGo* findTGo = (TextGo*)FindGo("Text");
	findTGo->SetActive(SCENE_MGR.TimeBaseFliper());

	SoundGo* sound = (SoundGo*)FindGo("MoveSound");
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Enter))
	{
		sound->sound.play();
		SCENE_MGR.ChangeScene(SceneId::Menu);
	}
}

void SceneTitile::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
