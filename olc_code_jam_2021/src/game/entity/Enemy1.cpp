#include "Enemy1.h"

namespace game
{
	Enemy1::Enemy1()
	{

	}

	Enemy1::~Enemy1()
	{

	}

	void Enemy1::Construct(nlohmann::json config, jam::IEntity* parent)
	{
		this->parent = parent;
		this->config = config;
	}

	void Enemy1::Draw(jam::IRenderer* render)
	{

	}

	void Enemy1::Update(jam::IScene* scene, float dt)
	{

	}

	bool Enemy1::IsDeleted()
	{
		return !this->isAlive;
	}

	void Enemy1::SetPosition(float x, float y)
	{

	}
}