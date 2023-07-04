#include "stdafx.h"
#include "VertexArrayGo.h"
#include "ResourceMgr.h"


VertexArrayGo::VertexArrayGo(const std::string id, const std::string n)
	:GameObject(n), textureId(id), texture(nullptr)
{
}

VertexArrayGo::~VertexArrayGo()
{
}

void VertexArrayGo::SetPosition(float x, float y)
{
	GameObject::SetPosition(x, y);
}

void VertexArrayGo::SetPosition(const sf::Vector2f& p)
{
	GameObject::SetPosition(p);
}

void VertexArrayGo::SetOrigin(Origins origin)
{
	GameObject::SetOrigin(origin);
}

void VertexArrayGo::SetOrigin(float x, float y)
{
	GameObject::SetOrigin(x,y);
}

void VertexArrayGo::Init()
{
	int line = 8;
	int width = 13;
	MakeMap(line, width, 100, {0,0});

	for (int i = 0; i < line*width; i++)
	{
		vertexArray[i* 4].texCoords = { 0.0 , 0.0 };
		vertexArray[i * 4+ 1].texCoords = { 50.0 , 0.0 };
		vertexArray[i * 4+ 2].texCoords = { 50.0 , 50.0 };
		vertexArray[i * 4+ 3].texCoords = { 0.0 , 50.0 };
	}
}

void VertexArrayGo::Release()
{
}

void VertexArrayGo::Reset()
{
	texture = RESOURCE_MGR.GetTexture(textureId);
}

void VertexArrayGo::Update(float dt)
{
}

void VertexArrayGo::Draw(sf::RenderWindow& window)
{
	window.draw(vertexArray, texture);
}

void VertexArrayGo::MakeMap(int height, int wide , int size,sf::Vector2f startPos)
{
	vertexArray.setPrimitiveType(sf::Quads);
	vertexArray.resize(height*wide*4);

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < wide; j++)
		{
			vertexArray[(4 * j) + (i * wide * 4)].position = { static_cast<float>(j) * size + startPos.x,static_cast<float>(i) * size + startPos.y };
			vertexArray[(4 * j) + (i * wide * 4) + 1].position = { static_cast<float>(j) * size + size + startPos.x,static_cast<float>(i) * size + startPos.y };
			vertexArray[(4 * j) + (i * wide * 4) + 2].position = { static_cast<float>(j) * size + size + startPos.x, static_cast<float>(i) * size + size + startPos.y };
			vertexArray[(4 * j) + (i * wide * 4) + 3].position = { static_cast<float>(j) * size + startPos.x,static_cast<float>(i) * size + size + startPos.y };
		}
	}
	

}
