#include "Bullet.h"
#include "../../jam/Shared.h"

namespace game
{
	Bullet::Bullet()
	{

	}

	Bullet::~Bullet()
	{

	}

	void Bullet::Construct(nlohmann::json config)
	{
		this->config = config;
		std::string imageFile = this->config["image"];
		if (!jam::backEnd->ResourceManager()->HasImage(jam::IMAGE_PATH + imageFile))
		{
			jam::backEnd->ResourceManager()->PreloadImage(jam::IMAGE_PATH + imageFile);
		}

		this->frame = 0;
		for (int i = 0; i < this->config["frames"].size(); i++)
		{
			if (this->config["frames"][i]["name"] == this->config["frame"])
			{
				this->frame = i;
				break;
			}
		}
		this->deleted = false;
	}

	void Bullet::Draw(jam::IRenderer* render)
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

	void Bullet::Update(jam::IScene* scene, float dt)
	{
		float dx, dy, speed;
		dx = (float)this->config["dx"].get<int>();
		dy = (float)this->config["dy"].get<int>();
		speed = this->config["speed"].get<float>();
		int screenW, screenH;
		scene->GetScreenSize(&screenW, &screenH);
		this->x += dx * speed * dt;
		this->y += dy * speed * dt;
		if (y < 0 || y >= screenH || x < 0 || x >= screenW)
		{
			// delete if out of bounds.
			this->deleted = true;
		}
	}

	bool Bullet::IsDeleted()
	{
		return deleted;
	}

	void Bullet::SetPosition(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
}