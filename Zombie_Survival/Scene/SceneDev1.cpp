#include "stdafx.h"
#include "SceneDev1.h"
#include "SceneMgr.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "GameObject.h"
#include "Framework.h"
#include "Player.h"
#include "VertexArrayGo.h"
SceneDev1::SceneDev1() : Scene(SceneId::Dev1), player(nullptr)
{
	//지역리소스 선언
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/player.png"));
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/background_sheet.png"));
}

SceneDev1::~SceneDev1()
{
}

void SceneDev1::Init()
{
	Release();

	player = (Player*)AddGo(new Player("graphics/player.png","Player"));

	VertexArrayGo* background = new VertexArrayGo("graphics/background_sheet.png", "Background");
	AddGo(background);
	for (auto go : gameObjects)
	{
		go->Init();
	}
}

void SceneDev1::Release()
{
	for (auto go : gameObjects)
	{
		delete go;
	}
}

void SceneDev1::Enter()
{
	Scene::Enter();
	player->sortLayer = 1;

	VertexArrayGo* background = (VertexArrayGo*)FindGo("Background");
	background->sortLayer = 0;
	//player->sprite.setTexture(*RESOURCE_MGR.GetTexture("graphics/player.png"));
}

void SceneDev1::Exit()
{
	Scene::Exit();
}

void SceneDev1::Update(float dt)
{
	Scene::Update(dt);
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Escape))
	{
		SCENE_MGR.ChangeScene(SceneId::Title);
	}
}

void SceneDev1::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

