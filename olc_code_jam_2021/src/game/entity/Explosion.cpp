#include <string>
#include "Explosion.h"
#include "../../jam/Shared.h"

namespace game
{
	Explosion::Explosion()
	{

	}

	Explosion::~Explosion()
	{

	}

	void Explosion::Construct(nlohmann::json config)
	{
		this->config = config;
		std::string imageFile = this->config["image"];
		if (!jam::backEnd->ResourceManager()->HasImage(jam::IMAGE_PATH + imageFile))
		{
			jam::backEnd->ResourceManager()->PreloadImage(jam::IMAGE_PATH + imageFile);
		}
		this->lifeTime = 0.0;
		this->frame = 0;
		this->deleted = false;
	}

	void Explosion::Draw(jam::IRenderer* render)
	{
		nlohmann::json frame = this->config["frames"][this->frame];
		float homeX, homeY, x, y, w, h;
		homeX = frame["home"]["x"].get<float>();
		homeY = frame["home"]["y"].get<float>();
		x = frame["x"].get<float>();
		y = frame["y"].get<float>();
		w = frame["width"].get<float>();
		h = frame["height"].get<float>();

		render->DrawSubImage(jam::backEnd->ResourceManager()->GetImage(jam::IMAGE_PATH + this->config["image"].get<std::string>()), this->x - homeX, this->y - homeY, x, y, w, h);
	}

	void Explosion::Update(jam::IScene* scene, float dt)
	{
		float animDelay = this->config["anim_delay"].get<float>();
		this->lifeTime += dt;
		if (this->lifeTime >= animDelay)
		{
			this->frame++;
			if (this->frame >= this->config["frames"].size())
			{
				this->deleted = true;
				this->frame = 0; // In case of a draw error.
			}
			this->lifeTime -= animDelay;
		}
	}

	bool Explosion::IsDeleted()
	{
		return deleted;
	}

	void Explosion::SetPosition(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
}