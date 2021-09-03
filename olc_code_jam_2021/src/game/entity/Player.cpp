#include "Player.h"

namespace game
{
	Player::Player()
	{

	}

	Player::~Player()
	{

	}

	void Player::Construct(nlohmann::json config)
	{
		this->config = config;
		std::string imageFile = this->config["image"];
		if (!jam::backEnd->ResourceManager()->HasImage(jam::IMAGE_PATH + imageFile))
		{
			jam::backEnd->ResourceManager()->PreloadImage(jam::IMAGE_PATH + imageFile);
		}
	}

	void Player::Draw(jam::IRenderer* render)
	{
		nlohmann::json currentFrame;
		for (int i = 0; i < this->config["frames"].size(); i++)
		{
			if (this->config["frames"][i]["name"] == this->frame)
			{
				currentFrame = this->config["frames"][i];
				break;
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

	void Player::GetHitBox(int* x, int* y, int* w, int* h)
	{
		nlohmann::json currentFrame;
		for (int i = 0; i < this->config["frames"].size(); i++)
		{
			if (this->config["frames"][i]["name"] == this->frame)
			{
				currentFrame = this->config["frames"][i];
				break;
			}
		}

		float homeX, homeY;
		homeX = currentFrame["home"]["x"].get<float>();
		homeY = currentFrame["home"]["y"].get<float>();
		*x = currentFrame["x"].get<float>() + this->x - homeX;
		*y = currentFrame["y"].get<float>() + this->y - homeY;
		*w = currentFrame["width"].get<float>();
		*h = currentFrame["height"].get<float>();
	}

	void Player::Update(jam::IScene* scene, float dt)
	{
		float moveSpeed;

		moveSpeed = this->config["speed"].get<float>();
		
		this->x += this->dx * moveSpeed * dt;
		this->y += this->dy * moveSpeed * dt;
	}

	bool Player::IsDeleted()
	{
		return false;
	}

	void Player::SetDirection(float dx, float dy)
	{
		this->dx = dx;
		this->dy = dy;
		if (this->dy < 0.0)
		{
			this->frame = "up";
		}
		else if (this->dy > 0.0)
		{
			this->frame = "down";
		}
		else
		{
			this->frame = "normal";
		}
	}

	void Player::SetPosition(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

}