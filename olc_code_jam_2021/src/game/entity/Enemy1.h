#pragma once
#ifndef __ENEMY_1_H__
#define __ENEMY_1_H__

#include "../../3rdParty/json/json.hpp"
#include "../../jam/IEntity.h"
#include "../../jam/IRenderer.h"
#include "../../jam/IScene.h"

namespace game
{
	class Enemy1 : public jam::IEntity
	{
	public:
		Enemy1();
		~Enemy1();
		void Construct(nlohmann::json config, jam::IEntity* parent);
		void Draw(jam::IRenderer* render) override;
		void Update(jam::IScene* scene, float dt) override;
		bool IsDeleted() override;
		void SetPosition(float x, float y) override;
	protected:
		bool isAlive;
		jam::IEntity* parent;
		nlohmann::json config;

	};
}
#endif