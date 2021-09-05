#pragma once

#ifndef __BOSS1_H__
#define __BOSS1_H__

#include "../../3rdParty//json/json.hpp"
#include "../../jam/IEntity.h"
#include "../../jam/IRenderer.h"
#include "../../jam/IScene.h"

namespace game
{

	enum Boss1State
	{
		ARRIVE, UP, DOWN, SHOOT, DESTROY
	};
	class Boss1 : public jam::IEntity
	{
	public:
		Boss1();
		~Boss1();
		void Construct(nlohmann::json config, int screenW, int screenH);
		void Draw(jam::IRenderer* render) override;
		void Update(jam::IScene* scene, float dt) override;
		bool IsDeleted() override;
		void SetPosition(float x, float y) override;
		void GetHitBox(int* x, int* y, int* w, int* h);
		void Hit();
	protected:
		bool isAlive;
		int hitPoints;
		nlohmann::json config;
		int frame;
		float shootDelay;
		float stateDuration;
		Boss1State state;
		float moveDist;
		nlohmann::json bulletConfig;
		float hitDelay;
	protected:
	};
}
#endif