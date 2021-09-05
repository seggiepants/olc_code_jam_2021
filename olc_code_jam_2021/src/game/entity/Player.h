#pragma once

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <string>
#include "../../3rdParty/json/json.hpp"
#include "../../jam/Configuration.h"
#include "../../jam/IEntity.h"
#include "../../jam/IRenderer.h"
#include "../../jam/IScene.h"
#include "Bullet.h"

namespace game
{

	enum PlayerState { PLAYER_NORMAL, PLAYER_EXPLODING };

	class Player : public jam::IEntity
	{
	public:
		Player();
		~Player() override;
		void Construct(nlohmann::json config, nlohmann::json bulletConfig);
		void Draw(jam::IRenderer* render) override;
		void GetHitBox(int* x, int* y, int* w, int* h);
		void Update(jam::IScene* scene, float dt) override;
		bool IsDeleted() override;
		void SetDirection(float dx, float dy);
		void SetPosition(float x, float y) override;
		PlayerState GetPlayerState() { return this->playerState; };
		game::Bullet* Shoot();
		void Hit();
	protected:
		float dx, dy, shotWait;
		PlayerState playerState;
		std::string frame;
		nlohmann::json config;
		nlohmann::json bulletConfig;
		bool deleted;
	};

}

#endif