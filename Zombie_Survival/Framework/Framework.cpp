#include "stdafx.h"
#include "Framework.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "ResourceMgr.h"

Framework::Framework(int w, int h, const std::string& t)
    :screenWidth(w), screenHeight(h), title(t)
{
}

void Framework::Init(int width, int height, const std::string& title)
{
	window.create(sf::VideoMode(width, height), title);

    //傈开府家胶 何福扁
    RESOURCE_MGR.Load(ResourceTypes::Texture, "graphics/icon.png");
    RESOURCE_MGR.Load(ResourceTypes::Texture, "graphics/background.png");
    RESOURCE_MGR.Load(ResourceTypes::Sound, "sound/select.wav");
    RESOURCE_MGR.Load(ResourceTypes::Font, "fonts/zombiecontrol.ttf");
    SCENE_MGR.Init();

 
}

void Framework::Release()
{
    //傈开府家胶 秦力
    RESOURCE_MGR.UnLoad(ResourceTypes::Texture, "graphics/icon.png");
    RESOURCE_MGR.UnLoad(ResourceTypes::Texture, "graphics/background.png");
    RESOURCE_MGR.UnLoad(ResourceTypes::Sound, "sound/select.wav");
    RESOURCE_MGR.UnLoad(ResourceTypes::Font, "fonts/zombiecontrol.ttf");

    SCENE_MGR.Release();
}

void Framework::Update(float dt)
{

    SCENE_MGR.Update(dt);
}

void Framework::Draw()
{
    SCENE_MGR.Draw(window);
}

void Framework::Run()
{
    Init(screenWidth, screenHeight,title);

    clock.restart();

    sf::Image cursorImg;
    cursorImg.loadFromFile("graphics/crosshair.png");

    sf::Cursor cursor;
    if (cursor.loadFromPixels(cursorImg.getPixelsPtr(), cursorImg.getSize(), { cursorImg.getSize().x / 2, cursorImg.getSize().y / 2 }))
    {
        window.setMouseCursor(cursor);
    }

    while (window.isOpen())
    {
        sf::Time deltaTime = clock.restart();
        float dt = deltaTime.asSeconds();

        INPUT_MGR.Clear();

        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                SCENE_MGR.GetCurrScene()->Exit();
                window.close();
                break;
            case sf::Event::GainedFocus:
                window.setMouseCursor(cursor);
                break;
            }
            INPUT_MGR.Update(event);
        }

        if (window.isOpen())
		{
			Update(dt);

			window.clear();
			Draw();
			window.display();
        }
       
    }
    Release();
}

sf::Vector2f Framework::GetWindowSize()
{
    return sf::Vector2f(screenWidth, screenHeight);
}

sf::RenderWindow& Framework::GetWindow()
{
    return window;
}
