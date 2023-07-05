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
}

SceneDev1::~SceneDev1()
{
}

void SceneDev1::Init()
{
	Release();
	sf::Vector2f windowSize = FRAMEWORK.GetWindowSize();
	sf::Vector2f centerPos = windowSize * 0.5f;

	worldView.setSize(windowSize);

	uiView.setSize(windowSize);
	uiView.setCenter(centerPos);

	
	VertexArrayGo* background = CreateBackground({ 30, 30 }, tilesize, { 50.f, 50.f }, "graphics/background_sheet.png");
	background->SetOrigin(Origins::MC);
	background->SetPosition(0.f,0.f);
	background->sortLayer = -1;
	AddGo(background);

	player = (Player*)AddGo(new Player("graphics/player.png", "Player"));
	AddGo(new SpriteGo("graphics/crosshair.png", "Crosshair"));
	CreateZombies(1000);
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
	
	worldView.setCenter(0.f, 0.f);
	player->SetPosition(0.f,0.f);

	SpriteGo* cursor = (SpriteGo*)FindGo("Crosshair");
	cursor->SetOrigin(Origins::MC);
}

void SceneDev1::Exit()
{
	ClearZombies();
	Scene::Exit();
}

void SceneDev1::Update(float dt)
{
	Scene::Update(dt);
	SpriteGo* cursor = (SpriteGo*)FindGo("Crosshair");
	cursor->SetPosition(SCENE_MGR.GetCurrScene()->ScreenToWorldPos(INPUT_MGR.GetMousePos()));

	worldView.setCenter(player->GetPosition());
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Escape))
	{
		SCENE_MGR.ChangeScene(SceneId::Title);
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num1))
	{
		SpawnZombies(10, player->GetPosition(), 700.f);
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num2))
	{
		ClearZombies();
	}

	if (zombies.empty())
	{
		SpawnZombies(10, player->GetPosition(), 700.f);
	}
}

void SceneDev1::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
	window.setMouseCursorVisible(false);
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

void SceneDev1::CreateZombies(int count)
{
	for (int i = 0; i < count; i++)
	{
		Zombie* zombie = new Zombie();
		Zombie::Types zombieType = (Zombie::Types)Utils::RandomRange(0, Zombie::TotalTypes-1);
		zombie->SetType(zombieType);
		zombie->SetPlayer(player);
		zombie->Init();
		zombie->SetActive(false);
		zombiePool.push_back(zombie);
	}
}

void SceneDev1::SpawnZombies(int count, sf::Vector2f center, float radius)
{
	for (int i = 0; i < count; ++i)
	{
		if (zombiePool.empty())
		{
			CreateZombies(count);
		}

		Zombie* zombie = zombiePool.front();
		zombiePool.pop_front();

		zombie->SetActive(true);

		sf::Vector2f pos;
		do
		{
			pos = center + Utils::RandomInCircle(radius);
		}
		while (Utils::Distance(center, pos) < 200.f && radius > 100.f);

		zombie->SetPosition(pos);

		zombies.push_back(zombie);

		zombie->Reset();
		AddGo(zombie);
	}
}

void SceneDev1::ClearZombies()
{
	for (auto zombie : zombies)
	{
		zombie->SetActive(false);
		RemoveGo(zombie);
		zombiePool.push_back(zombie);
	}
	zombies.clear();
}

void SceneDev1::OnDieZombie(Zombie* zombie)
{
	zombies.remove(zombie);
	zombie->SetActive(false);
	zombiePool.push_back(zombie);
}

std::list<Zombie*>* SceneDev1::GetZombieList()
{
	return &zombies;
}

sf::Vector2f SceneDev1::GetMapTop()
{
	VertexArrayGo* background = (VertexArrayGo*)FindGo("Background");
	sf::FloatRect back = background->vertexArray.getBounds();
	return { back.left + tilesize.x, back.top + tilesize.y };
}

sf::Vector2f SceneDev1::GetMapBot()
{
	VertexArrayGo* background = (VertexArrayGo*)FindGo("Background");
	sf::FloatRect back = background->vertexArray.getBounds();
	return{ back.left + back.width - tilesize.x,back.top + back.height - tilesize.y };
}

