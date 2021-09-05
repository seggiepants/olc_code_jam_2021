#include "Enemy1.h"
#include "../../jam/Shared.h"

namespace game
{
	Enemy1::Enemy1()
	{
		this->parent = nullptr;
		this->config = nullptr;
	}

	Enemy1::~Enemy1()
	{

	}

	void Enemy1::Construct(nlohmann::json config, jam::IEntity* parent)
	{
		this->parent = parent;
		this->config = config;
		this->isAlive = true; 
		this->animDelay = 0.0;
		std::string imageFile = this->config["image"];
		if (!jam::backEnd->ResourceManager()->HasImage(jam::IMAGE_PATH + imageFile))
		{
			jam::backEnd->ResourceManager()->PreloadImage(jam::IMAGE_PATH + imageFile);
		}
	}

	void Enemy1::Draw(jam::IRenderer* render)
	{
		nlohmann::json currentFrame;
		std::string frame = std::to_string(this->frame);
		for (int i = 0; i < this->config["frames"].size(); i++)
		{
			if (this->config["frames"][i]["name"] == frame)
			{
				currentFrame = this->config["frames"][i];
			}
		}

		float homeX, homeY, x, y, w, h;
		homeX = currentFrame["home"]["x"].get<float>();
		homeY = currentFrame["home"]["y"].get<float>();
		x = currentFrame["x"].get<float>();
		y = currentFrame["y"].get<float>();
		w = currentFrame["width"].get<float>();
		h = currentFrame["height"].get<float>();
		render->DrawSubImage(jam::backEnd->ResourceManager()->GetImage(jam::IMAGE_PATH + this->config["image"].get<std::string>()), this->x - homeX, this->y - homeY, x, y, w, h);

	}

	void Enemy1::Update(jam::IScene* scene, float dt)
	{
		if (this->isAlive)
		{
			std::string frame;
			if (this->animDelay <= 0)
			{
				this->frame++;
				if (this->frame >= this->config["frames"].size())
				{
					this->frame = 0;
				}
				this->animDelay = this->config["anim_delay"].get<float>();
			}
			else 
			{
				animDelay -= dt;
			}
			frame = std::to_string(this->frame);

			this->x -= this->config["speed"].get<float>() * dt;
			nlohmann::json currentFrame;
			for (int i = 0; i < this->config["frames"].size(); i++)
			{
				if (this->config["frames"][i]["name"] == frame)
				{
					currentFrame = this->config["frames"][i];
					break;
				}
			}
			int width = currentFrame["width"].get<int>();
			if (this->x + (float)width  < 0.0)
			{
				if (this->parent != nullptr)
				{
					// Send deallocate message -- soon.

				}
				this->isAlive = false;
			}
		}
	}

	bool Enemy1::IsDeleted()
	{
		return !this->isAlive;
	}

	void Enemy1::SetPosition(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	void Enemy1::Respawn(float x, float y)
	{
		this->SetPosition(x, y);
		this->isAlive = true;
		this->animDelay = 0.0;
	}

	void Enemy1::GetHitBox(int* x, int* y, int* w, int* h)
	{
		nlohmann::json currentFrame;
		std::string frame = std::to_string(this->frame);
		for (int i = 0; i < this->config["frames"].size(); i++)
		{
			if (this->config["frames"][i]["name"] == frame)
			{
				currentFrame = this->config["frames"][i];
				break;
			}
		}

		float homeX, homeY;
		homeX = currentFrame["home"]["x"].get<int>();
		homeY = currentFrame["home"]["y"].get<int>();
		*x = currentFrame["x"].get<int>() + this->x - homeX;
		*y = currentFrame["y"].get<int>() + this->y - homeY;
		*w = currentFrame["width"].get<int>();
		*h = currentFrame["height"].get<int>();
	}
}