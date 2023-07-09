#include "stdafx.h"
#include "SceneGame.h"
#include "SceneMgr.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "GameObject.h"
#include "Framework.h"

#include "Player.h"
#include "Zombie.h"
#include "VertexArrayGo.h"
#include "RectGo.h"
#include "TextGo.h"
#include "SpriteEffect.h"
#include "Item.h"
#include "SoundGo.h"

SceneGame::SceneGame() : Scene(SceneId::Game), player(nullptr),
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
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/pistol.png"));
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/mg.png"));
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/change.png"));
	resources.push_back(std::make_tuple(ResourceTypes::Sound, "sound/hit.wav"));
	resources.push_back(std::make_tuple(ResourceTypes::Sound, "sound/pickup.wav"));
	resources.push_back(std::make_tuple(ResourceTypes::Sound, "sound/splat.wav"));
	resources.push_back(std::make_tuple(ResourceTypes::Sound, "sound/powerup.wav"));
	resources.push_back(std::make_tuple(ResourceTypes::Sound, "sound/zombie_die.wav"));
	resources.push_back(std::make_tuple(ResourceTypes::Sound, "sound/player_hit.wav"));
	resources.push_back(std::make_tuple(ResourceTypes::Sound, "sound/player_death.wav"));
	resources.push_back(std::make_tuple(ResourceTypes::Sound, "sound/bgm.wav"));
	resources.push_back(std::make_tuple(ResourceTypes::Sound, "sound/shoot.wav"));
	resources.push_back(std::make_tuple(ResourceTypes::Sound, "sound/reload.wav"));
	resources.push_back(std::make_tuple(ResourceTypes::Sound, "sound/reload_failed.wav"));
	resources.push_back(std::make_tuple(ResourceTypes::Sound, "sound/swap.wav"));
}

SceneGame::~SceneGame()
{
	Release();
}

void SceneGame::Init()
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
	AddGo(new TextGo("Frame"));
	AddGo(new TextGo("GameOver"));
	AddGo(new SpriteGo("graphics/ammo_icon.png","AmmoIcon"));
	AddGo(new SpriteGo("graphics/background.png", "ShopBack"));
	AddGo(new SpriteGo("graphics/pistol.png", "Pistol"));
	AddGo(new SpriteGo("graphics/mg.png", "Mg"));
	AddGo(new SpriteGo("graphics/change.png", "Change"));
	AddGo(new SoundGo("sound/hit.wav","BulletToZombieHit"));
	AddGo(new SoundGo("sound/pickup.wav","ItemGet"));
	AddGo(new SoundGo("sound/splat.wav","ItemDrop"));
	AddGo(new SoundGo("sound/select.wav","ShopIn"));
	AddGo(new SoundGo("sound/powerup.wav","PowerUp"));
	AddGo(new SoundGo("sound/zombie_die.wav","ZombieDie"));
	AddGo(new SoundGo("sound/player_hit.wav","ZombieToPlayerHit"));
	AddGo(new SoundGo("sound/player_death.wav", "PlayerDie"));
	AddGo(new SoundGo("sound/bgm.wav","Bgm"));
	AddGo(new SoundGo("sound/shoot.wav", "SShoot"));
	AddGo(new SoundGo("sound/reload.wav", "SReload"));
	AddGo(new SoundGo("sound/reload_failed.wav", "SReloadFail"));
	AddGo(new SoundGo("sound/swap.wav", "Swap"));

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
	player->SetGunSound((SoundGo*)FindGo("SShoot"),
		(SoundGo*)FindGo("SReloadFail"),
		(SoundGo*)FindGo("SReload"));

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

void SceneGame::Release()
{
	zombiePool.Release();
	bloodEffectPool.Release();
	for (auto go : gameObjects)
	{
		delete go;
	}
}

void SceneGame::Enter()
{
	Scene::Enter();
	hiScore = SCENE_MGR.hiScore;
	score = 0;
	wave = 0;
	pause = true;
	isGameOver = false;
	player->SetPosition(0.f,0.f);
	padeIn = 0;

	itemAmmo->SetType(Item::Types::Ammo);
	itemAmmo->sortOrder = -2;
	itemAmmo->SetActive(false);
	itemHealth->SetType(Item::Types::Hp);
	itemHealth->sortOrder = -1;
	itemHealth->SetActive(false);

	RectGo* hp = (RectGo*)FindGo("Hp");
	hp->SetOrigin(Origins::ML);
	hp->rectangle.setFillColor(sf::Color::Red);
	hp->SetPosition(FRAMEWORK.GetWindowSize().x * 0.25f, FRAMEWORK.GetWindowSize().y -60.f);
	hp->sortLayer = 100;

	RectGo* reload = (RectGo*)FindGo("Reload");
	reload->rectangle.setFillColor(sf::Color::Blue);
	reload->sortLayer = 5;
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

	findSGo = (SpriteGo*)FindGo("Pistol");
	findSGo->SetOrigin(Origins::MC);
	findSGo->SetPosition(FRAMEWORK.GetWindowSize().x / 2, 40);
	findSGo->SetSize(0.5f, 0.5f);
	findSGo->sortLayer = 100;

	findSGo = (SpriteGo*)FindGo("Mg");
	findSGo->SetOrigin(Origins::MC);
	findSGo->SetPosition(FRAMEWORK.GetWindowSize().x / 2, 40);
	findSGo->SetSize(0.5f, 0.5f);
	findSGo->sortLayer = 100;
	findSGo->SetActive(false);

	findSGo = (SpriteGo*)FindGo("Change");
	findSGo->SetOrigin(Origins::MC);
	findSGo->SetPosition(FRAMEWORK.GetWindowSize().x / 2, 80);
	findSGo->SetSize(1.5f, 1.5f);
	findSGo->sortLayer = 100;

	findTGo = (TextGo*)FindGo("Shop");
	findTGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/zombiecontrol.ttf"));
	findTGo->text.setString("\n\n1- INCREASED RATE OF FIRE\n2- INCREASED CLIP SIZE 2\n3- INCREASED MAX HEALTH\n4- INCREASED RUN SPEED\n5- MORE AND BETTER HEALTH PICKUPS\n6- MORE AND BETTER AMMO PICKUPS");
	findTGo->text.setCharacterSize(60);
	findTGo->text.setFillColor(sf::Color(255, 255, 255, 0));
	Utils::SetOrigin(findTGo->text, Origins::TL);
	findTGo->text.setPosition(100.f,50.f);
	findTGo->sortLayer = 102;

	findTGo = (TextGo*)FindGo("Frame");
	findTGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/zombiecontrol.ttf"));
	findTGo->text.setCharacterSize(60);
	findTGo->text.setFillColor(sf::Color::White);
	Utils::SetOrigin(findTGo->text, Origins::TL);
	findTGo->text.setPosition(100.f, 100.f);
	findTGo->sortLayer = 103;
	findTGo->SetActive(false);

	findTGo = (TextGo*)FindGo("GameOver");
	findTGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/zombiecontrol.ttf"));
	findTGo->text.setCharacterSize(100);
	findTGo->text.setFillColor(sf::Color::White);
	findTGo->text.setString("G A M E O V E R");
	Utils::SetOrigin(findTGo->text, Origins::MC);
	findTGo->text.setPosition(FRAMEWORK.GetWindowSize().x / 2, FRAMEWORK.GetWindowSize().y / 2);
	findTGo->sortLayer = 104;
	findTGo->SetActive(false);

	SoundGo* bgm = (SoundGo*)FindGo("Bgm");
	bgm->sound.setLoop(true);
}

void SceneGame::Exit()
{
	ClearObjectPool(bloodEffectPool);
	ClearObjectPool(zombiePool);
	player->Reset();
	itemAmmo->Reset();
	itemHealth->Reset();
	Scene::Exit();
}

void SceneGame::Update(float dt)
{
	//메뉴로 돌아가기
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Escape))
	{
		SCENE_MGR.ChangeScene(SceneId::Title);
		return;
	}
	worldView.setCenter(player->GetPosition());

	//게임오버로 중단 및 메뉴복귀
	if (isGameOver)
	{
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::Enter))
		{
			SCENE_MGR.ChangeScene(SceneId::Menu);
			return;
		}
		return;
	}

	//일시정지(시작 및 업그레이드)
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
	
	//좀비 전부 처치시
	if (zombiePool.GetUseList().size() == 0)
	{
		WaveEnd();
	}

	//UI 업데이트 관련
	FrameOut(dt);
	UiUpdate();
	//TestCode();
	//1 for spawn, 2 for next wave
}

void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

VertexArrayGo* SceneGame::CreateBackground(sf::Vector2f size, sf::Vector2f tileSize, sf::Vector2f texSize, std::string textureId)
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

void SceneGame::SpawnZombies(int count, sf::Vector2f center, float radius)
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
void SceneGame::OnDieZombie(Zombie* zombie)
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

void SceneGame::OnDiePlayer()
{
	isGameOver = true;
	wave = 0;
	SoundGo* FSound = (SoundGo*)FindGo("PlayerDie");
	FSound->Play();
	FSound = (SoundGo*)FindGo("Bgm");
	FSound->sound.stop();
	TextGo* findTGo = (TextGo*)FindGo("GameOver");
	findTGo->SetActive(true);
	if (SCENE_MGR.hiScore < hiScore)
	{
		SaveBestSco();
	}
	
}

const std::list<Zombie*>* SceneGame::GetZombieList() const
{
	return &zombiePool.GetUseList();
}

void SceneGame::StartGame()
{
	pause = false;
	TextGo* findTGo = (TextGo*)FindGo("Score");
	findTGo->text.setPosition(20.f, 20.f);
	findTGo->text.setString("SCORE:0");
	findTGo->text.setCharacterSize(60);
	SoundGo* findSound = (SoundGo*)FindGo("Bgm");
	findSound->sound.play();
}

void SceneGame::Shop(float dt)
{
	SoundGo* findSound = (SoundGo*)FindGo("PowerUp");
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num1))
	{
		player->GunUp(1);
		findSound->sound.play();
		pause = false;
	}
	else if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num2))
	{
		player->GunUp(2);
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

void SceneGame::TestCode()
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

void SceneGame::FrameOut(float dt)
{
	frame++;
	totalDt += dt;

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::F))
	{
		TextGo* findTGo = (TextGo*)FindGo("Frame");
		frameCheck = !frameCheck;
		findTGo->SetActive(frameCheck);
	}

	if (totalDt >= 1.f) // 1초가 되었을때
	{
		totalDt -= 1.f;
		std::string str = std::to_string(frame);
		TextGo* findTGo = (TextGo*)FindGo("Frame");
		findTGo->text.setString(str);
		frame = 0.f;
	}
}

void SceneGame::UiUpdate()
{

	RectGo* hp = (RectGo*)FindGo("Hp");
	hp->SetSize({ player->GetHpBarLength() * 3.f, 30.f });

	bool ammoUiUpdateCheck = INPUT_MGR.GetMouseButtonDown(sf::Mouse::Button::Left) ||
		INPUT_MGR.GetMouseButton(sf::Mouse::Button::Left) ||
		INPUT_MGR.GetKeyDown(sf::Keyboard::Tab);
	if (ammoUiUpdateCheck || player->GetReloadStatus() != ReloadStatus::NONE)
	{
		if (player->GetReloadStatus() == ReloadStatus::START)
		{
			RectGo* reload = (RectGo*)FindGo("Reload");
			reload->SetActive(true);
			reload->SetSize({ player->GetReloadTimer() * 30.f, 5.f });
			reload->SetOrigin(Origins::MC);
			reload->SetPosition(player->GetPosition().x, player->GetPosition().y + 15.f);
		}
		AmmoUiUpdate();

		if (INPUT_MGR.GetKeyDown(sf::Keyboard::Tab))
		{
			SoundGo* swap = (SoundGo*)FindGo("Swap");
			swap->Play();
			int guntype = player->GetCurrentGun();
			switch (guntype)
			{
			case 0:
			{
				SpriteGo* gun = (SpriteGo*)FindGo("Pistol");
				gun->SetActive(true);
				gun = (SpriteGo*)FindGo("Mg");
				gun->SetActive(false);
				break;
			}
			case 1:
			{
				SpriteGo* gun = (SpriteGo*)FindGo("Mg");
				gun->SetActive(true);
				gun = (SpriteGo*)FindGo("Pistol");
				gun->SetActive(false);
				break;
			}
			}
			RectGo* reload = (RectGo*)FindGo("Reload");
			reload->SetActive(false);
		}
		if (player->GetReloadStatus() == ReloadStatus::END)
		{
			RectGo* reload = (RectGo*)FindGo("Reload");
			reload->SetActive(false);
			player->SetReloadStatus(ReloadStatus::NONE);
		}

	}
}

void SceneGame::SaveBestSco()
{
	std::ofstream outputFile("ZSsave.dat", std::ios::binary | std::ios::out);

	if (!outputFile)
	{
		std::cout << "세이브 생성 오류" << std::endl;
		return;
	}
	outputFile.write((char*)&hiScore, sizeof(hiScore));

	outputFile.close();

	std::cout << "최고점수가 저장되었습니다." << std::endl;
	SCENE_MGR.hiScore = hiScore;
}

void SceneGame::WaveEnd()
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

void SceneGame::ShopOff(float dt)
{
	padeIn -= dt * 300;
	SoundGo* findSound = (SoundGo*)FindGo("Bgm");
	findSound->sound.setVolume(100);
	SpriteGo* findSGo = (SpriteGo*)FindGo("ShopBack");
	findSGo->sprite.setColor(sf::Color(255, 255, 255, padeIn));
	TextGo* findTGo = (TextGo*)FindGo("Shop");
	findTGo->text.setFillColor(sf::Color(255, 255, 255, padeIn));
}


void SceneGame::AmmoUiUpdate()
{
	TextGo* findTGo = (TextGo*)FindGo("RemainAmmo");
	std::stringstream ss = player->GetAmmoInfo();
	findTGo->text.setString(ss.str());
}