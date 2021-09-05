#include "Player.h"

namespace game
{
	Player::Player()
	{

	}

	Player::~Player()
	{

	}

	void Player::Construct(nlohmann::json config, nlohmann::json bulletConfig)
	{
		this->config = config;
		this->bulletConfig = bulletConfig;
		std::string imageFile = this->config["image"];
		if (!jam::backEnd->ResourceManager()->HasImage(jam::IMAGE_PATH + imageFile))
		{
			jam::backEnd->ResourceManager()->PreloadImage(jam::IMAGE_PATH + imageFile);
		}
		this->deleted = false;
		this->playerState = PLAYER_NORMAL;
	}

	void Player::Hit()
	{
		this->playerState = PLAYER_EXPLODING;
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
		if (this->playerState == PLAYER_NORMAL)
		{
			this->x += this->dx * moveSpeed * dt;
			this->y += this->dy * moveSpeed * dt;

			if (this->shotWait > 0)
				this->shotWait -= dt;
		}
		else if (this->playerState == PLAYER_EXPLODING)
		{
			int screenW, screenH;
			scene->GetScreenSize(&screenW, &screenH);
			this->x += this->dx * moveSpeed * dt;
			this->y += moveSpeed * dt; // Fall down.
			if (this->y > screenH)
			{
				this->deleted = true;

			}
		}
	}

	bool Player::IsDeleted()
	{
		return this->deleted;
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

	game::Bullet* Player::Shoot()
	{
		if (this->shotWait <= 0 && !this->IsDeleted())
		{
			this->shotWait = this->config["shot_delay"].get<float>();
			Bullet* bullet = new Bullet();
			bullet->Construct(this->bulletConfig);

			nlohmann::json currentFrame;
			for (int i = 0; i < this->config["frames"].size(); i++)
			{
				if (this->config["frames"][i]["name"] == this->frame)
				{
					currentFrame = this->config["frames"][i];
					break;
				}
			}
			if (currentFrame != nullptr)
			{
				float muzzelX, muzzelY;
				muzzelX = (float)currentFrame["muzzle"]["x"].get<int>();
				muzzelY = (float)currentFrame["muzzle"]["y"].get<int>();
				float homeX, homeY;
				homeX = (float)currentFrame["home"]["x"].get<int>();
				homeY = (float)currentFrame["home"]["y"].get<int>();
				bullet->SetPosition(this->GetX() + muzzelX - homeX, this->GetY() + muzzelY - homeY);
				jam::backEnd->ResourceManager()->GetAudio(jam::SOUND_PATH + "zap.wav")->Play();
			}
			return bullet;
		}
		return nullptr;
	}

}