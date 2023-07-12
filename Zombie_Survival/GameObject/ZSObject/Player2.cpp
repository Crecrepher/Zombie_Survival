#include "stdafx.h"
#include "Player2.h"
#include "InputMgr.h"
#include "RectGo.h"
#include "Framework.h"
#include "ResourceMgr.h"
Player2::Player2(const std::string& textureId, const std::string& n)
	:SpriteGo(textureId,n)
{
}

Player2::~Player2()
{
}

void Player2::Init()
{
	//RESOURCE_MGR.Load(ResourceTypes::AnimationClip, "animations/ruby/Idle_Side.csv");
	//RESOURCE_MGR.Load(ResourceTypes::AnimationClip, "animations/ruby/Idle_Up.csv");
	//RESOURCE_MGR.Load(ResourceTypes::AnimationClip, "animations/ruby/Idle_Down.csv");
	//RESOURCE_MGR.Load(ResourceTypes::AnimationClip, "animations/ruby/Move_Side.csv");
	//RESOURCE_MGR.Load(ResourceTypes::AnimationClip, "animations/ruby/Move_Up.csv");
	//RESOURCE_MGR.Load(ResourceTypes::AnimationClip, "animations/ruby/Move_Down.csv");
	//
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/ruby/Idle_Side.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/ruby/Idle_Up.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/ruby/Idle_Down.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/ruby/Move_Side.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/ruby/Move_Up.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/ruby/Move_Down.csv"));
	
	animation.SetTarget(&sprite);
	SetOrigin(Origins::MC);
}

void Player2::Reset()
{	
	animation.Play("Idle_Down");
	SetOrigin(origin);
	SetPosition(0, 0);
}

void Player2::Update(float dt)
{
	//이동
	direction.x = INPUT_MGR.GetAxis(Axis::Horizontal);
	direction.y = INPUT_MGR.GetAxis(Axis::Vertical);
	float magnitude = Utils::Magnitude(direction);
	if (magnitude > 1.f)
	{
		direction /= magnitude;
	}
	
	position += direction * speed * dt;
	SetPosition(position);



	//애니메이션 - 대각선 이동시 좌,우 우선
	if(direction.x != 0 || direction.y != 0)
	{
		if (animation.GetCurrentClipId() != "Move_Side" && 
			INPUT_MGR.GetAxisRaw(Axis::Horizontal) != 0.f)
		{
			animation.Play("Move_Side");
		}
		else if (animation.GetCurrentClipId() != "Move_Down" && 
			INPUT_MGR.GetAxisRaw(Axis::Vertical) >0.f &&
			INPUT_MGR.GetAxisRaw(Axis::Horizontal) == 0.f)
		{
			animation.Play("Move_Down");
		}
		else if (animation.GetCurrentClipId() != "Move_Up" && 
			INPUT_MGR.GetAxisRaw(Axis::Vertical) < 0.f &&
			INPUT_MGR.GetAxisRaw(Axis::Horizontal) == 0.f)
		{
			animation.Play("Move_Up");
		}
	}
	else
	{
		if (animation.GetCurrentClipId() == "Move_Side"&& animation.GetCurrentClipId() != "Idle_Side")
		{
			animation.Play("Idle_Side");
		}
		else if (animation.GetCurrentClipId() == "Move_Down" && animation.GetCurrentClipId() != "Idle_Down")
		{
			animation.Play("Idle_Down");
		}
		else if (animation.GetCurrentClipId() == "Move_Up" && animation.GetCurrentClipId() != "Idle_Up")
		{
			animation.Play("Idle_Up");
		}
	}
	if (INPUT_MGR.GetAxisRaw(Axis::Horizontal) > 0)
	{
		animation.GetTarget()->setScale(-1, 1);
	}
	else if (INPUT_MGR.GetAxisRaw(Axis::Horizontal) < 0)
	{
		animation.GetTarget()->setScale(1, 1);
	}
	animation.Update(dt);
}

sf::FloatRect Player2::GetGlobalBounds() const
{
	return animation.GetTarget()->getGlobalBounds();
}
