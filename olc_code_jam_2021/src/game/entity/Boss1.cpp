#include "Boss1.h"
#include "Bullet.h"
#include "../../jam/Configuration.h"
#include "../../jam/IImage.h"
#include "../../jam/Shared.h"
#include "../scene/SceneGame.h"

namespace game
{
	Boss1::Boss1()
	{

	}

	Boss1::~Boss1()
	{

	}

	void Boss1::Construct(nlohmann::json config, int screenW, int screenH)
	{
		this->config = config;

		this->hitPoints = this->config["hitpoints"].get<int>();
		this->state = Boss1State::ARRIVE;
		int width = this->config["frames"][0]["width"].get<int>();
		int height = this->config["frames"][0]["height"].get<int>();
		this->SetX(screenW + width + 1);
		this->SetY(screenH / 2);
		this->bulletConfig = jam::Configuration::LoadJsonFile(jam::CONFIG_PATH + "enemy1_bullet.json");
	}

	void Boss1::Draw(jam::IRenderer* render)
	{
		nlohmann::json currentFrame = this->config["frames"][0];

		float homeX, homeY, x, y, w, h;
		homeX = currentFrame["home"]["x"].get<float>();
		homeY = currentFrame["home"]["y"].get<float>();
		x = currentFrame["x"].get<float>();
		y = currentFrame["y"].get<float>();
		w = currentFrame["width"].get<float>();
		h = currentFrame["height"].get<float>();

		render->DrawSubImage(jam::backEnd->ResourceManager()->GetImage(jam::IMAGE_PATH + this->config["image"].get<std::string>()), this->x - homeX, this->y - homeY, x, y, w, h);
		if (this->state == Boss1State::UP || this->state == Boss1State::DOWN)
		{
			jam::IImage* shieldImg = jam::backEnd->ResourceManager()->GetImage(jam::IMAGE_PATH + "boss1_shield.png");
			x = y = 0;
			w = shieldImg->GetWidth();
			h = shieldImg->GetHeight();
			homeX = w / 2;
			homeY = h / 2;
			render->DrawSubImage(shieldImg, this->x - homeX, this->y - homeY, x, y, w, h);
		}
	}

	void Boss1::Update(jam::IScene* scene, float dt)
	{
		int screenW, screenH;
		if (this->IsDeleted() == false)
		{
			scene->GetScreenSize(&screenW, &screenH);
			int yMin = this->config["ymin"].get<int>();
			int yMax = this->config["ymax"].get<int>();
			float speed = this->config["speed"].get<float>();
			if (hitDelay > 0.0)
			{
				this->hitDelay -= dt;
			}
			if (this->state == Boss1State::ARRIVE)
			{
				int targetX = (2 * screenW) / 3;
				if (this->x <= targetX)
				{
					// Switch to up/down;
					this->moveDist = 64 + (rand() % (yMax - yMin - 64));
					if (rand() % 2 == 0)
					{
						this->state = Boss1State::UP;
					}
					else
					{
						this->state = Boss1State::DOWN;
					}
					this->stateDuration = this->config["state_duration"].get<float>();
				}
				else
				{
					this->x -= dt * speed;
				}

			}
			else if (this->state == Boss1State::UP || this->state == Boss1State::DOWN)
			{
				float dy;
				float speed = this->config["speed"].get<float>();
				this->stateDuration -= dt;
				int yMin = this->config["ymin"].get<int>();
				int yMax = this->config["ymax"].get<int>();
				if (this->state == Boss1State::UP)
				{
					dy = -1;
				}
				else
				{
					dy = 1;
				}
				this->y += dy * speed * dt;
				if (this->y <= yMin || this->y >= yMax || this->stateDuration <= 0.0)
				{
					this->state = Boss1State::SHOOT;
					this->stateDuration = this->config["state_duration"].get<float>();
					this->shootDelay = this->config["shoot_delay"].get<float>();
				}
			}
			else if (this->state == Boss1State::SHOOT)
			{
				this->stateDuration -= dt;
				this->shootDelay -= dt;
				if (this->shootDelay <= 0)
				{
					int width = this->config["frames"][0]["width"].get<int>();
					int height = this->config["frames"][0]["height"].get<int>();
					int halfHeight = height / 2;
					int halfWidth = width / 2;
					int bx = this->x - halfHeight;
					int by = 0;
					int spawnCount = this->config["spawn_count"].get<int>();
					for (int i = 0; i < spawnCount; i++)
					{
						by = this->y - halfHeight + (rand() % height);
						Bullet* b = new Bullet();
						b->Construct(this->bulletConfig);
						b->SetPosition(bx, by);
						((SceneGame*)scene)->SpawnEnemyBullet(b);
					}
					this->shootDelay = this->config["shoot_delay"].get<float>();
				}
				if (this->stateDuration <= 0)
				{
					// Switch to up/down;
					this->moveDist = 64 + (rand() % (yMax - yMin - 64));
					int screenW, screenH;
					scene->GetScreenSize(&screenW, &screenH);
					if (this->y >= screenH / 2)
					{
						this->state = Boss1State::UP;
					}
					else
					{
						this->state = Boss1State::DOWN;
					}
					this->stateDuration = this->config["state_duration"].get<float>();
				}
			}
		}
	}
	bool Boss1::IsDeleted()
	{
		return this->hitPoints <= 0;
	}

	void Boss1::SetPosition(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	void Boss1::GetHitBox(int* x, int* y, int* w, int* h)
	{
		nlohmann::json currentFrame = this->config["frames"][0];

		int homeX, homeY;
		homeX = currentFrame["home"]["x"].get<int>();
		homeY = currentFrame["home"]["y"].get<int>();
		*x = this->x - homeX;
		*y = this->y - homeY;
		*w = currentFrame["width"].get<int>();
		*h = currentFrame["height"].get<int>();
	}

	void Boss1::Hit()
	{
		if (hitDelay <= 0)
		{
			this->hitPoints--;
			this->hitDelay = this->config["hit_delay"].get<float>();
		}
	}
}