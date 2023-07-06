#include "stdafx.h"
#include "SceneDev1.h"
#include "SceneMgr.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "GameObject.h"
#include "Framework.h"

#include "Player.h"
#include "Zombie.h"
#include "VertexArrayGo.h"
#include "RectGo.h"
#include "Blood.h"

SceneDev1::SceneDev1() : Scene(SceneId::Dev1), player(nullptr)
{
	//지역리소스 선언
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/player.png"));
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/background_sheet.png"));
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/bloater.png"));
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/chaser.png"));
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/crawler.png"));
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/bullet.png"));
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/crosshair.png"));
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/blood.png"));
}

SceneDev1::~SceneDev1()
{
	Release();
}

void SceneDev1::Init()
{
	Release();

	// 카메라
	sf::Vector2f windowSize = FRAMEWORK.GetWindowSize();
	sf::Vector2f centerPos = windowSize * 0.5f;

	worldView.setSize(windowSize);
	uiView.setSize(windowSize);
	uiView.setCenter(centerPos);

	sf::Vector2f tileWorldSize = { 50.f, 50.f };
	sf::Vector2f tileTexSize = { 50.f, 50.f };

	player = (Player*)AddGo(new Player("graphics/player.png", "Player"));
	VertexArrayGo* background = CreateBackground({ 30, 30 }, tileWorldSize, tileTexSize, "graphics/background_sheet.png");
	AddGo(background);
	AddGo(new RectGo("Hp"));

	for (auto go : gameObjects)
	{
		go->Init();
	}


	background->SetOrigin(Origins::MC);
	background->SetPosition(0.f, 0.f);
	background->sortLayer = -1;

	wallBounds = background->vertexArray.getBounds();
	wallBounds.width -= tileWorldSize.x * 2.f;
	wallBounds.height -= tileWorldSize.y * 2.f;
	wallBounds.left += tileWorldSize.x;
	wallBounds.top += tileWorldSize.y;

	player->SetWallBounds(wallBounds);

	zombiePool.OnCreate = [this](Zombie* zombie) {
		Zombie::Types zombieType = (Zombie::Types)Utils::RandomRange(0,Zombie::TotalTypes-1);
		zombie->SetType(zombieType);
		zombie->SetPlayer(player);
	};
	zombiePool.Init();

	ObjectPool<Blood>* ptr = &bloodPool;
	bloodPool.OnCreate = [this,ptr](Blood* blood) {
		blood->textureId = "graphics/blood.png";
		blood->pool = ptr;
	};
	bloodPool.Init();
}

void SceneDev1::Release()
{
	bloodPool.Release();
	zombiePool.Release();
	for (auto go : gameObjects)
	{
		delete go;
	}
}

void SceneDev1::Enter()
{
	Scene::Enter();

	/*worldView.setCenter(0.f, 0.f);*/
	isGameOver = false;
	player->SetPosition(0.f,0.f);

	RectGo* hp = (RectGo*)FindGo("Hp");
	hp->SetOrigin(Origins::ML);
	hp->rectangle.setFillColor(sf::Color::Red);
	hp->SetPosition(FRAMEWORK.GetWindowSize().x * 0.25f, FRAMEWORK.GetWindowSize().y * 0.9f);
	hp->sortLayer = 100;
}

void SceneDev1::Exit()
{
	ClearBloods();
	ClearZombies();
	player->Reset();

	Scene::Exit();
}

void SceneDev1::Update(float dt)
{
	Scene::Update(dt);

	if (isGameOver)
	{
		SCENE_MGR.ChangeScene(sceneId);
		return;
	}
	worldView.setCenter(player->GetPosition());
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Escape))
	{
		SCENE_MGR.ChangeScene(SceneId::Title);
		return;
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num1))
	{
		SpawnZombies(10, player->GetPosition(), 1000.f);
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num2))
	{
		ClearZombies();
	}

	if (zombiePool.GetUseList().size() == 0)
	{
		SpawnZombies(10, player->GetPosition(), 1000.f);
	}

	RectGo* hp = (RectGo*)FindGo("Hp");
	hp->SetSize({ player->GetHp() * 3.f, 30.f });
}

void SceneDev1::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

VertexArrayGo* SceneDev1::CreateBackground(sf::Vector2f size, sf::Vector2f tileSize, sf::Vector2f texSize, std::string textureId)
{
	VertexArrayGo* background = new VertexArrayGo(textureId, "Background");
	sf::Vector2f startPos = { 0,0 };

	background->vertexArray.setPrimitiveType(sf::Quads);
	background->vertexArray.resize(size.x * size.y * 4);

	sf::Vector2f offsets[4] =
	{
		{0.f,0.f},
		{tileSize.x,0.f},
		{tileSize.x,tileSize.y },
		{0.f,tileSize.y}
	};

	sf::Vector2f texOffsets[4] =
	{
		{0.f,0.f},
		{texSize.x,0.f},
		{texSize.x,texSize.y },
		{0.f,texSize.y}
	};

	sf::Vector2f currPos = startPos;
	for (int i = 0; i < size.y; ++i)
	{
		for (int j = 0; j < size.x; ++j)
		{
			int texIndex = 3;
			if (i != 0 && i != size.y - 1 && j != 0 && j != size.x - 1)
			{
				texIndex = Utils::RandomRange(0, 2);
			}

			int tileIndex = size.x * i + j;
			for (int k = 0; k < 4; ++k)
			{
				int vertexIndex = tileIndex * 4 + k;
				background->vertexArray[vertexIndex].position = currPos + offsets[k];
				background->vertexArray[vertexIndex].texCoords = texOffsets[k];
				background->vertexArray[vertexIndex].texCoords.y += texSize.y * texIndex;
			}
			currPos.x += tileSize.x;
		}
		currPos.x = startPos.x;
		currPos.y += tileSize.y;

		
	}
	return background;
}

void SceneDev1::SpawnZombies(int count, sf::Vector2f center, float radius)
{
	sf::Vector2f wallBoundsLT = { wallBounds.left,wallBounds.top };
	sf::Vector2f wallBoundsRB = { wallBounds.left + wallBounds.width,wallBounds.top + wallBounds.height };
	for (int i = 0; i < count; ++i)
	{
		Zombie* zombie = zombiePool.Get();
		sf::Vector2f pos;
		do
		{
			pos = center + Utils::RandomInCircle(radius);
			if (!wallBounds.contains(pos))
			{
				pos = Utils::Clamp(pos, wallBoundsLT, wallBoundsRB);
			}
		}
		while ((Utils::Distance(center, pos) < 700.f && radius > 100.f));
	
		zombie->SetPosition(pos);
		AddGo(zombie);
	}
}

void SceneDev1::ClearZombies()
{
	for (auto zombie : zombiePool.GetUseList())
	{
		RemoveGo(zombie);
	}
	zombiePool.Clear();
}

void SceneDev1::ClearBloods()
{
	for (auto blood : bloodPool.GetUseList())
	{
		RemoveGo(blood);
	}
	bloodPool.Clear();
}

void SceneDev1::OnDieZombie(Zombie* zombie)
{
	Blood* blood = bloodPool.Get();
	blood->SetPosition(zombie->GetPosition());
	AddGo(blood);
	float bloodsize = 0.5 + (1.0-((int)zombie->GetType() * 0.5));
	float rot = Utils::RandomRange(0, 270);
	blood->SetSize(bloodsize, bloodsize);
	blood->sprite.setRotation(rot);

	RemoveGo(zombie);
	zombiePool.Return(zombie);
}

void SceneDev1::OnDiePlayer()
{
	isGameOver = true;
}

const std::list<Zombie*>* SceneDev1::GetZombieList() const
{
	return &zombiePool.GetUseList();
}
