#pragma once

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <string>
#include "../../3rdParty/json/json.hpp"
#include "../../jam/Configuration.h"
#include "../../jam/IEntity.h"
#include "../../jam/IRenderer.h"
#include "../../jam/IScene.h"

namespace game
{
	class Player : public jam::IEntity
	{
	public:
		Player();
		~Player() override;
		void Construct(std::string fileName);
		void Draw(jam::IRenderer* render) override;
		void Update(jam::IScene* scene, float dt) override;
		bool IsDeleted() override;
		void SetDirection(float dx, float dy);
		void SetPosition(float x, float y) override;
	protected:
		float dx, dy, shotWait;
		std::string frame;
		nlohmann::json config;
	};

}

#endif