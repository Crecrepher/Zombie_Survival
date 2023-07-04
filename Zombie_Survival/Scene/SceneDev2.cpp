#include "stdafx.h"
#include "SceneDev2.h"
#include "SceneMgr.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "GameObject.h"
#include "Framework.h"

SceneDev2::SceneDev2() : Scene(SceneId::Game)
{
	//지역리소스 선언
	//resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/player4.png"));
}

SceneDev2::~SceneDev2()
{
}

void SceneDev2::Init()
{
	Release();
	//GO선언
	/*AddGo(new PFUnitGo("Player"));*/

	for (auto go : gameObjects)
	{
		go->Init();
	}
}

void SceneDev2::Release()
{
	for (auto go : gameObjects)
	{
		delete go;
	}
}

void SceneDev2::Enter()
{
	Scene::Enter();
	//GO조정
	//BouncyBall* ball = (BouncyBall*)FindGo("Ball");
	//ball->SetBallSize(10.f);
	//ball->SetPosition(100.f, 100.f);
}

void SceneDev2::Exit()
{
	Scene::Exit();
}

void SceneDev2::Update(float dt)
{
	Scene::Update(dt);
	std::cout << INPUT_MGR.GetMousePos().x << std::endl;
}

void SceneDev2::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

