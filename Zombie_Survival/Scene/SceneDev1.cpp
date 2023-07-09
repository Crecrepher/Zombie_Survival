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
#include "TextGo.h"
#include "SpriteEffect.h"
#include "Item.h"
#include "SoundGo.h"

SceneDev1::SceneDev1() : Scene(SceneId::Dev1), player(nullptr),
itemHealth(nullptr), itemAmmo(nullptr)
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
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/ammo_icon.png"));
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/ammo_pickup.png"));
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/health_pickup.png"));
	resources.push_back(std::make_tuple(ResourceTypes::Sound, "sound/hit.wav"));
	resources.push_back(std::make_tuple(ResourceTypes::Sound, "sound/pickup.wav"));
	resources.push_back(std::make_tuple(ResourceTypes::Sound, "sound/splat.wav"));
	resources.push_back(std::make_tuple(ResourceTypes::Sound, "sound/powerup.wav"));
	resources.push_back(std::make_tuple(ResourceTypes::Sound, "sound/shoot.wav"));
	resources.push_back(std::make_tuple(ResourceTypes::Sound, "sound/zombie_die.wav"));
	resources.push_back(std::make_tuple(ResourceTypes::Sound, "sound/player_hit.wav"));
	resources.push_back(std::make_tuple(ResourceTypes::Sound, "sound/bgm.wav"));
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
	player->sortLayer = 1;
	
	itemHealth = (Item*)AddGo(new Item("graphics/health_pickup.png", "ItemHealth"));
	itemHealth->SetPlayer(player);
	itemAmmo = (Item*)AddGo(new Item("graphics/ammo_pickup.png", "ItemAmmo"));
	itemAmmo->SetPlayer(player);
	VertexArrayGo* background = CreateBackground({ 30, 30 }, tileWorldSize, tileTexSize, "graphics/background_sheet.png");
	
	AddGo(background);
	AddGo(new RectGo("Hp"));
	AddGo(new RectGo("Reload"));
	AddGo(new TextGo("Score"));
	AddGo(new TextGo("HiScore"));
	AddGo(new TextGo("RemainAmmo"));
	AddGo(new TextGo("WaveNZombies"));
	AddGo(new TextGo("Shop"));
	AddGo(new SpriteGo("graphics/ammo_icon.png","AmmoIcon"));
	AddGo(new SpriteGo("graphics/background.png", "ShopBack"));
	AddGo(new SoundGo("BulletToZombieHit"));
	AddGo(new SoundGo("ItemGet"));
	AddGo(new SoundGo("ItemDrop"));
	AddGo(new SoundGo("ShopIn"));
	AddGo(new SoundGo("PowerUp"));
	AddGo(new SoundGo("Shoot"));
	AddGo(new SoundGo("ZombieDie"));
	AddGo(new SoundGo("ZombieToPlayerHit"));
	AddGo(new SoundGo("Bgm"));

	for (auto go : gameObjects)
	{
		go->Init();
	}

	itemHealth->SetSound((SoundGo*)FindGo("ItemGet"), (SoundGo*)FindGo("ItemDrop"));
	itemAmmo->SetSound((SoundGo*)FindGo("ItemGet"), (SoundGo*)FindGo("ItemDrop"));

	background->SetOrigin(Origins::MC);
	background->SetPosition(0.f, 0.f);
	background->sortLayer = -1;

	wallBounds = background->vertexArray.getBounds();
	wallBounds.width -= tileWorldSize.x * 2.f;
	wallBounds.height -= tileWorldSize.y * 2.f;
	wallBounds.left += tileWorldSize.x;
	wallBounds.top += tileWorldSize.y;

	player->SetWallBounds(wallBounds);
	player->SetSound((SoundGo*)FindGo("ZombieToPlayerHit"));

	zombiePool.OnCreate = [this](Zombie* zombie) {
		Zombie::Types zombieType = (Zombie::Types)Utils::RandomRange(0,Zombie::TotalTypes-1);
		zombie->SetType(zombieType);
		zombie->SetPlayer(player);
		zombie->SetSound((SoundGo*)FindGo("BulletToZombieHit"));
	};
	zombiePool.Init();

	bloodEffectPool.OnCreate = [this](SpriteEffect* effect) {
		effect->textureId = "graphics/blood.png";
		effect->SetDuration(3.f);
		effect->SetPool(&bloodEffectPool);
	};
	bloodEffectPool.Init();
}

void SceneDev1::Release()
{
	zombiePool.Release();
	bloodEffectPool.Release();
	for (auto go : gameObjects)
	{
		delete go;
	}
}

void SceneDev1::Enter()
{
	Scene::Enter();

	score = 0;
	wave = 0;
	pause = true;
	isGameOver = false;
	player->SetPosition(0.f,0.f);
	padeIn = 0;

	SoundGo* findSound = (SoundGo*)FindGo("BulletToZombieHit");
	findSound->sound.setBuffer(*RESOURCE_MGR.GetSoundBuffer("sound/hit.wav"));

	findSound = (SoundGo*)FindGo("ItemGet");
	findSound->sound.setBuffer(*RESOURCE_MGR.GetSoundBuffer("sound/pickup.wav"));

	findSound = (SoundGo*)FindGo("ItemDrop");
	findSound->sound.setBuffer(*RESOURCE_MGR.GetSoundBuffer("sound/splat.wav"));

	findSound = (SoundGo*)FindGo("ShopIn");
	findSound->sound.setBuffer(*RESOURCE_MGR.GetSoundBuffer("sound/select.wav"));
	findSound = (SoundGo*)FindGo("PowerUp");
	findSound->sound.setBuffer(*RESOURCE_MGR.GetSoundBuffer("sound/powerup.wav"));

	findSound = (SoundGo*)FindGo("Shoot");
	findSound->sound.setBuffer(*RESOURCE_MGR.GetSoundBuffer("sound/shoot.wav"));

	findSound = (SoundGo*)FindGo("ZombieDie");
	findSound->sound.setBuffer(*RESOURCE_MGR.GetSoundBuffer("sound/zombie_die.wav"));

	findSound = (SoundGo*)FindGo("ZombieToPlayerHit");
	findSound->sound.setBuffer(*RESOURCE_MGR.GetSoundBuffer("sound/player_hit.wav"));

	findSound = (SoundGo*)FindGo("Bgm");
	findSound->sound.setBuffer(*RESOURCE_MGR.GetSoundBuffer("sound/bgm.wav"));
	findSound->sound.setLoop(true);

	itemAmmo->SetType(Item::Types::Ammo);
	itemAmmo->sortOrder = -2;
	itemHealth->SetType(Item::Types::Hp);
	itemAmmo->sortOrder = -1;

	RectGo* hp = (RectGo*)FindGo("Hp");
	hp->SetOrigin(Origins::ML);
	hp->rectangle.setFillColor(sf::Color::Red);
	hp->SetPosition(FRAMEWORK.GetWindowSize().x * 0.25f, FRAMEWORK.GetWindowSize().y -60.f);
	hp->sortLayer = 100;

	RectGo* reload = (RectGo*)FindGo("Reload");
	reload->SetOrigin(Origins::MC);
	reload->rectangle.setFillColor(sf::Color::Blue);
	reload->sortLayer = 100;
	reload->SetActive(false);

	TextGo* findTGo = (TextGo*)FindGo("Score");
	findTGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/zombiecontrol.ttf"));
	findTGo->text.setString("\nPRESS ENTER\nTO CONTINUE");
	findTGo->text.setCharacterSize(130);
	findTGo->text.setFillColor(sf::Color::White);
	Utils::SetOrigin(findTGo->text, Origins::TL);
	findTGo->text.setPosition(50.f, 50.f);
	findTGo->sortLayer = 100;

	findTGo = (TextGo*)FindGo("HiScore");
	findTGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/zombiecontrol.ttf"));
	std::stringstream ss;
	ss << "HI SCORE:" << hiScore;
	findTGo->text.setString(ss.str());
	findTGo->text.setCharacterSize(60);
	findTGo->text.setFillColor(sf::Color::White);
	Utils::SetOrigin(findTGo->text, Origins::MR);
	findTGo->text.setPosition(FRAMEWORK.GetWindowSize().x - 100.f, 50.f);
	findTGo->sortLayer = 100;

	findTGo = (TextGo*)FindGo("RemainAmmo");
	findTGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/zombiecontrol.ttf"));
	ss.str("");
	ss.clear();
	ss = player->GetAmmoInfo();
	findTGo->text.setString(ss.str());
	findTGo->text.setCharacterSize(60);
	findTGo->text.setFillColor(sf::Color::White);
	Utils::SetOrigin(findTGo->text, Origins::ML);
	findTGo->text.setPosition(100.f, FRAMEWORK.GetWindowSize().y - 50.f);
	findTGo->sortLayer = 100;

	findTGo = (TextGo*)FindGo("WaveNZombies");
	findTGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/zombiecontrol.ttf"));
	findTGo->text.setString("WAVE:0\tZOMBIES:10");
	findTGo->text.setCharacterSize(60);
	findTGo->text.setFillColor(sf::Color::White);
	Utils::SetOrigin(findTGo->text, Origins::MR);
	findTGo->text.setPosition(FRAMEWORK.GetWindowSize().x - 50.f, FRAMEWORK.GetWindowSize().y - 50.f);
	findTGo->sortLayer = 100;

	SpriteGo* findSGo = (SpriteGo*)FindGo("AmmoIcon");
	findSGo->SetOrigin(Origins::ML);
	findSGo->SetPosition(30.f, FRAMEWORK.GetWindowSize().y - 40.f);
	findSGo->sortLayer = 100;

	findSGo = (SpriteGo*)FindGo("ShopBack");
	findSGo->SetOrigin(Origins::TL);
	findSGo->sprite.setScale(FRAMEWORK.GetWindowSize().x/1920.f, FRAMEWORK.GetWindowSize().y / 1080.f);
	findSGo->SetPosition(0, 0);
	findSGo->sprite.setColor(sf::Color(255,255,255,0));
	findSGo->sortLayer = 101;

	findTGo = (TextGo*)FindGo("Shop");
	findTGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/zombiecontrol.ttf"));
	findTGo->text.setString("\n\n1- INCREASED RATE OF FIRE\n2- INCREASED CLIP SIZE 2\n3- INCREASED MAX HEALTH\n4- INCREASED RUN SPEED\n5- MORE AND BETTER HEALTH PICKUPS\n6- MORE AND BETTER AMMO PICKUPS");
	findTGo->text.setCharacterSize(60);
	findTGo->text.setFillColor(sf::Color(255, 255, 255, 0));
	Utils::SetOrigin(findTGo->text, Origins::TL);
	findTGo->text.setPosition(100.f,50.f);
	findTGo->sortLayer = 102;
}

void SceneDev1::Exit()
{
	ClearObjectPool(bloodEffectPool);
	ClearObjectPool(zombiePool);
	player->Reset();
	itemAmmo->Reset();
	itemHealth->Reset();
	Scene::Exit();
}

void SceneDev1::Update(float dt)
{
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Escape))
	{
		SCENE_MGR.ChangeScene(SceneId::Title);
		return;
	}
	worldView.setCenter(player->GetPosition());

	if (pause)
	{
		if (wave > 1 && pause)
		{
			Shop(dt);
		}
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::Enter)&& wave < 1)
		{
			StartGame();
		}
		return;
	}
	else if (wave > 1 && !pause && padeIn > 0)
	{
		ShopOff(dt);
	}

	Scene::Update(dt);

	if (isGameOver)
	{
		SCENE_MGR.ChangeScene(sceneId);
		return;
	}
	
	TestCode();

	if (zombiePool.GetUseList().size() == 0)
	{
		WaveEnd();
	}

	RectGo* hp = (RectGo*)FindGo("Hp");
	hp->SetSize({ player->GetHpBarLength() * 3.f, 30.f });

	bool ammoUiUpdateCheck = INPUT_MGR.GetMouseButtonDown(sf::Mouse::Button::Left) ||
		INPUT_MGR.GetMouseButton(sf::Mouse::Button::Left) ||
		INPUT_MGR.GetKeyDown(sf::Keyboard::Q) ||
		INPUT_MGR.GetKeyDown(sf::Keyboard::E);
	if (ammoUiUpdateCheck || player->GetReloadStatus() != ReloadStatus::NONE)
	{
		RectGo* reload = (RectGo*)FindGo("Reload");
		reload->SetActive(true);
		reload->SetSize({ player->GetReloadTimer() * 30.f, 5.f});
		reload->SetPosition(player->GetPosition().x, player->GetPosition().y + 10.f);

		//SoundGo* findSound = (SoundGo*)FindGo("Shoot");
		//findSound->sound.play();
		AmmoUiUpdate();

		if (player->GetReloadStatus() == ReloadStatus::END)
		{
			reload->SetActive(false);
			player->SetReloadStatus(ReloadStatus::NONE);
		}
	}
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
		zombie->sortLayer = 2;
		AddGo(zombie);
	}
}
void SceneDev1::OnDieZombie(Zombie* zombie)
{
	SoundGo* findSound = (SoundGo*)FindGo("ZombieDie");
	findSound->sound.play();

	SpriteEffect* bloodE = bloodEffectPool.Get();
	bloodE->SetPosition(zombie->GetPosition());
	AddGo(bloodE);
	float bloodsize = 0.5 + (1.0 - ((int)zombie->GetType() * 0.5));
	float rot = Utils::RandomRange(0, 270);
	bloodE->SetSize(bloodsize, bloodsize);
	bloodE->sprite.setRotation(rot);
	bloodE->sprite.setRotation(rot);

	score += 20-(5*(int)zombie->GetType());
	TextGo* findTGo = (TextGo*)FindGo("Score");
	std::stringstream ss;
	ss << "SCORE:" << score;
	findTGo->text.setString(ss.str());
	if (score > hiScore)
	{
		hiScore = score;
		findTGo = (TextGo*)FindGo("HiScore");
		ss.str("");
		ss.clear();
		findTGo->text.setFillColor(sf::Color::Green);
		ss << "HI SCORE:" << hiScore;
		findTGo->text.setString(ss.str());
	}

	findTGo = (TextGo*)FindGo("WaveNZombies");
	ss.str("");
	ss.clear();
	ss << "WAVE:" << wave << "\t" << "ZOMBIES:" << zombiePool.GetUseList().size()-1;
	findTGo->text.setString(ss.str());

	if (!itemAmmo->GetSpawn())
	{
		itemAmmo->TryMake(zombie->GetPosition());
	}
	if (!itemHealth->GetSpawn())
	{
		itemHealth->TryMake(zombie->GetPosition());
	}
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

void SceneDev1::StartGame()
{
	pause = false;
	TextGo* findTGo = (TextGo*)FindGo("Score");
	findTGo->text.setPosition(20.f, 20.f);
	findTGo->text.setString("SCORE:0");
	findTGo->text.setCharacterSize(60);
	SoundGo* findSound = (SoundGo*)FindGo("Bgm");
	findSound->sound.play();
}

void SceneDev1::Shop(float dt)
{
	SoundGo* findSound = (SoundGo*)FindGo("PowerUp");
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num1))
	{

		findSound->sound.play();
		pause = false;
	}
	else if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num2))
	{

		findSound->sound.play();
		pause = false;
	}
	else if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num3))
	{
		player->HealthUp();
		findSound->sound.play();
		pause = false;
	}
	else if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num4))
	{
		player->SpeedUp();
		findSound->sound.play();
		pause = false;
	}
	else if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num5))
	{
		itemHealth->ItemUpgrade();
		findSound->sound.play();
		pause = false;
	}
	else if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num6))
	{
		itemAmmo->ItemUpgrade();
		findSound->sound.play();
		pause = false;
	}
	if (padeIn < 254)
	{
		padeIn += dt * 100;
		SpriteGo* findSGo = (SpriteGo*)FindGo("ShopBack");
		findSGo->sprite.setColor(sf::Color(255, 255, 255, padeIn));
		TextGo* findTGo = (TextGo*)FindGo("Shop");
		findTGo->text.setFillColor(sf::Color(255, 255, 255, padeIn));
	}
}

void SceneDev1::TestCode()
{
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num1))
	{
		SpawnZombies(10 + wave * 5, player->GetPosition(), 1000.f);
		wave++;
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num2))
	{
		ClearObjectPool(zombiePool);
	}
}

void SceneDev1::WaveEnd()
{
	SpawnZombies(10 + wave * 5, player->GetPosition(), 1000.f);
	wave++;
	TextGo* findTGo = (TextGo*)FindGo("WaveNZombies");
	std::stringstream ss;
	ss << "WAVE:" << wave << "\t" << "ZOMBIES:" << zombiePool.GetUseList().size();
	findTGo->text.setString(ss.str());
	SoundGo* findSound = (SoundGo*)FindGo("ShopIn");
	findSound->sound.play();
	findSound = (SoundGo*)FindGo("Bgm");
	if (wave > 1)
	{
		findSound->sound.setVolume(20);
		pause = true;
	}
}

void SceneDev1::ShopOff(float dt)
{
	padeIn -= dt * 300;
	SoundGo* findSound = (SoundGo*)FindGo("Bgm");
	findSound->sound.setVolume(100);
	SpriteGo* findSGo = (SpriteGo*)FindGo("ShopBack");
	findSGo->sprite.setColor(sf::Color(255, 255, 255, padeIn));
	TextGo* findTGo = (TextGo*)FindGo("Shop");
	findTGo->text.setFillColor(sf::Color(255, 255, 255, padeIn));
}


void SceneDev1::AmmoUiUpdate()
{
	TextGo* findTGo = (TextGo*)FindGo("RemainAmmo");
	std::stringstream ss = player->GetAmmoInfo();
	findTGo->text.setString(ss.str());
}