#pragma once
#include "Scene.h"

class SceneMenu : public Scene
{
protected:
	int menuIndex;
	float closeTimer;
	bool isClose;
	int hiScore = 0;
	bool loading;
public:
	SceneMenu();
	virtual ~SceneMenu() override;

	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void MenuMove();
	void MenuSelect();
};

