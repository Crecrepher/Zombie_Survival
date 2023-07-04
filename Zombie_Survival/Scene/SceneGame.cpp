#include "stdafx.h"
#include "SceneGame.h"
#include "SceneMgr.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "GameObject.h"
#include "Framework.h"

SceneGame::SceneGame() : Scene(SceneId::Game)
{
	//지역리소스 선언
	//resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/player4.png"));
}

SceneGame::~SceneGame()
{
}

void SceneGame::Init()
{
	Release();
	//GO선언
	/*AddGo(new PFUnitGo("Player"));*/

	for (auto go : gameObjects)
	{
		go->Init();
	}
}

void SceneGame::Release()
{
	for (auto go : gameObjects)
	{
		delete go;
	}
}

void SceneGame::Enter()
{
	Scene::Enter();
	//GO조정
	//BouncyBall* ball = (BouncyBall*)FindGo("Ball");
	//ball->SetBallSize(10.f);
	//ball->SetPosition(100.f, 100.f);
}

void SceneGame::Exit()
{
	Scene::Exit();
}

void SceneGame::Update(float dt)
{
	Scene::Update(dt);
}

void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

