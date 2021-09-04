#pragma once

#ifndef __BULLET_H__
#define __BULLET_H__

#include "../../3rdParty/json/json.hpp"
#include "../../jam/IEntity.h"
#include "../../jam/IScene.h"
#include "../../jam/IRenderer.h"

namespace game
{
	class Bullet : public jam::IEntity
	{
	public:
		Bullet();
		~Bullet();
		void Construct(nlohmann::json config);
		void Draw(jam::IRenderer* render) override;
		void Update(jam::IScene* scene, float dt) override;
		bool IsDeleted() override;
		void SetPosition(float x, float y) override;
	protected:
		nlohmann::json config;
		int frame;
		bool deleted;

	};
}
#endif