#pragma once
#ifndef __ENEMY_1_H__
#define __ENEMY_1_H__

#include <string>
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
		void Respawn(float x, float y);
		void GetHitBox(int* x, int* y, int* w, int* h);
		void Hit() { this->isAlive = false; }
	protected:
		bool isAlive;
		jam::IEntity* parent;
		nlohmann::json config;
		int frame;
		float animDelay;

	};
}
#endif