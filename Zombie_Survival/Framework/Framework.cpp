#include "stdafx.h"
#include "Framework.h"
#include "InputMgr.h"
#include "SceneMgr.h"
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
    RESOURCE_MGR.Load(ResourceTypes::Sound, "sound/select.wav");

    SCENE_MGR.Init();

 
}

void Framework::Release()
{
    //傈开府家胶 秦力
    RESOURCE_MGR.UnLoad(ResourceTypes::Texture, "graphics/icon.png");
    RESOURCE_MGR.UnLoad(ResourceTypes::Sound, "sound/select.wav");

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
    while (window.isOpen())
    {
        sf::Time deltaTime = clock.restart();
        float dt = deltaTime.asSeconds();

        INPUT_MGR.Clear();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            INPUT_MGR.Update(event);
        }

        Update(dt);
        window.clear();
        //window.draw(shape);
        Draw();
        window.display();
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
