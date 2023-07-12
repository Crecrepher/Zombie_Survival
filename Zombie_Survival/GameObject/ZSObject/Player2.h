#pragma once
#include "SpriteGo.h"
#include "AnimatioControler.h"
class RectGo;

class Player2 : public SpriteGo
{
protected:
	AnimatioControler animation;
	sf::Vector2f direction = { 0.f,0.f };
	float speed = 1000.f;

	bool stand = true;
	bool move = false;


public:
	Player2(const std::string& textureId = "", const std::string& n = "");
	virtual ~Player2() override;

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;

	sf::FloatRect GetGlobalBounds() const;
};

