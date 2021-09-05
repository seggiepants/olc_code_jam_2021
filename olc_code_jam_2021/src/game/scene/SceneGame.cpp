#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include "../../jam/Collision.h"
#include "../../jam/IAudio.h"
#include "../../jam/IImage.h"
#include "../../jam/IResourceManager.h"
#include "../../jam/Rect.h"
#include "SceneGame.h"
#include "../../jam/SceneManager.h"
#include "../../jam/Shared.h"
#include "../GameAssets.h"
#include "../../jam/TileMapLayer.h"

namespace game
{
	const float SCROLL_SPEED = 100;
	const float MESSAGE_SPEED = 100;

	SceneGame::SceneGame()
	{
		this->nextScene = nullptr;
		this->screenWidth = this->screenHeight = 0;
		this->tileMap = nullptr;
		this->player = nullptr;
	}

	SceneGame::~SceneGame()
	{
		this->ClearObjects();
	}

	void SceneGame::ClearObjects()
	{
		if (this->tileMap != nullptr)
		{
			delete this->tileMap;
		}
		this->tileMap = nullptr;

		if (this->player != nullptr)
		{
			delete this->player;
			this->player = nullptr;
		}

		for (std::vector<game::Explosion*>::iterator iter = this->explosions.begin(); iter != this->explosions.end(); iter++)
		{
			delete (*iter);
			*iter = nullptr;
		}
		this->explosions.clear();

		for (std::vector<game::Enemy1*>::iterator iter = this->enemies.begin(); iter != this->enemies.end(); iter++)
		{
			delete (*iter);
			*iter = nullptr;
		}
		this->enemies.clear();

		for (std::vector<game::Bullet*>::iterator iter = this->playerBullets.begin(); iter != this->playerBullets.end(); iter++)
		{
			delete (*iter);
			*iter = nullptr;
		}
		this->playerBullets.clear();

		for (std::vector<game::Bullet*>::iterator iter = this->enemyBullets.begin(); iter != this->enemyBullets.end(); iter++)
		{
			delete (*iter);
			*iter = nullptr;
		}
		this->enemyBullets.clear();		

	}

	void SceneGame::Construct(int screenWidth, int screenHeight)
	{
		this->screenWidth = screenWidth;
		this->screenHeight = screenHeight;
		this->gameTime = 0.0;
		this->gameState = GameState::PLAY;

		this->nextScene = (IScene*)this;

		if (!jam::backEnd->ResourceManager()->HasAudio(jam::SOUND_PATH + "explosion.wav"))
			jam::backEnd->ResourceManager()->PreloadAudio(jam::SOUND_PATH + "explosion.wav");
		if (!jam::backEnd->ResourceManager()->HasAudio(jam::SOUND_PATH + "win.wav"))
			jam::backEnd->ResourceManager()->PreloadAudio(jam::SOUND_PATH + "win.wav");
		if (!jam::backEnd->ResourceManager()->HasAudio(jam::SOUND_PATH + "lose.wav"))
			jam::backEnd->ResourceManager()->PreloadAudio(jam::SOUND_PATH + "lose.wav");
		if (!jam::backEnd->ResourceManager()->HasAudio(jam::SOUND_PATH + "zap.wav"))
			jam::backEnd->ResourceManager()->PreloadAudio(jam::SOUND_PATH + "zap.wav");



		
		this->player = new Player();
		this->player->Construct(jam::Configuration::LoadJsonFile(jam::CONFIG_PATH + "player.json"), jam::Configuration::LoadJsonFile(jam::CONFIG_PATH + "player_bullet.json"));
		this->player->SetPosition(screenWidth / 2.0, screenHeight / 2.0);
		this->enemyConfig = jam::Configuration::LoadJsonFile(jam::CONFIG_PATH + "enemy1.json");
		this->enemyBulletConfig = jam::Configuration::LoadJsonFile(jam::CONFIG_PATH + "enemy1_bullet.json");
		this->explosionConfig = jam::Configuration::LoadJsonFile(jam::CONFIG_PATH + "explosion.json");
		this->tileMap = new jam::TileMap();
		std::string fileName = jam::MAP_PATH;
		fileName = fileName + "map1.json";
		this->offset = 0.0;
		this->dx = 1;
		this->tileMap->Construct(fileName);
		this->NextLevel();
	}

	void SceneGame::Draw(jam::IRenderer* render)
	{
		int width, height;
		jam::rgb bg(0, 0, 0, 255);
		jam::rgb fg(255, 255, 255, 255);

		render->Clear(bg);
		render->GetScreenSize(&width, &height);
		this->screenWidth = width;
		this->screenHeight = height;

		render->Clear(bg);
		this->tileMap->Draw(render, this->offset, 0, 0, 0, 640, 480);
		float halfW, halfH;
		jam::IFont* fontSmall = jam::backEnd->ResourceManager()->GetFont(game::FONT_SMALL);
		halfW = this->screenWidth / 2.0;
		halfH = this->screenHeight / 2.0;
		/*
		std::string msg = "Press <ESCAPE> to";
		int y;
		fontSmall->MeasureText(msg, &width, &height);
		y = 100; // ((screenHeight - height) / 2) - height;
		fontSmall->DrawText(render, msg, (screenWidth - width) / 2, y, fg);
		msg = " return to the menu";
		fontSmall->MeasureText(msg, &width, &height);
		y += height * 2;
		fontSmall->DrawText(render, msg, (screenWidth - width) / 2, y, fg);
		*/


		for (int i = 0; i < this->explosions.size(); i++)
		{
			this->explosions[i]->Draw(render);
		}
		for (int i = 0; i < this->playerBullets.size(); i++)
		{
			this->playerBullets[i]->Draw(render);
		}

		this->player->Draw(render);

		for (int i = 0; i < this->enemies.size(); i++)
		{
			this->enemies[i]->Draw(render);
		}
		for (int i = 0; i < this->enemyBullets.size(); i++)
		{
			this->enemyBullets[i]->Draw(render);
		}

		if (this->gameState == GameState::PAUSE)
		{
			int width, height;
			std::string msg = "PAUSE";
			int y;
			fontSmall->MeasureText(msg, &width, &height);
			y = ((screenHeight - height) / 2);
			fontSmall->DrawText(render, msg, (screenWidth - width) / 2, y, fg);
		}
		else if (this->gameState == GameState::GAME_OVER)
		{
			std::string msg = "GAME OVER";
			fontSmall->MeasureText(msg, &width, &height);			
			fontSmall->DrawText(render, msg, (screenWidth - width) / 2, this->messageY, fg);
		}
		else if (this->gameState == GameState::NEXT_LEVEL)
		{
			std::string msg = "LEVEL COMPLETE";
			fontSmall->MeasureText(msg, &width, &height);
			fontSmall->DrawText(render, msg, (screenWidth - width) / 2, this->messageY, fg);
		}
	}

	void SceneGame::GetScreenSize(int* screenWidth, int* screenHeight)
	{
		*screenWidth = this->screenWidth;
		*screenHeight = this->screenHeight;
	}

	void SceneGame::JoystickButtonDown(int id, jam::JoystickButton btn)
	{
		if (id == 0)
		{
			if (btn == jam::JoystickButton::A)
			{
				this->joyA = true;
			}

			if (btn == jam::JoystickButton::DPAD_LEFT)
			{
				this->joyLeft = true;
			}

			if (btn == jam::JoystickButton::DPAD_RIGHT)
			{
				this->joyRight = true;
			}

			if (btn == jam::JoystickButton::DPAD_UP)
			{
				this->joyUp = true;
			}

			if (btn == jam::JoystickButton::DPAD_DOWN)
			{
				this->joyDown = true;
			}
		}

	}

	void SceneGame::JoystickButtonUp(int id, jam::JoystickButton btn)
	{
		if (id == 0)
		{
			if (btn == jam::JoystickButton::A)
			{
				this->joyA = false;
			}

			if (btn == jam::JoystickButton::DPAD_LEFT)
			{
				this->joyLeft = false;
			}

			if (btn == jam::JoystickButton::DPAD_RIGHT)
			{
				this->joyRight = false;
			}

			if (btn == jam::JoystickButton::DPAD_UP)
			{
				this->joyUp = false;
			}

			if (btn == jam::JoystickButton::DPAD_DOWN)
			{
				this->joyDown = false;
			}

			if (btn == jam::JoystickButton::START)
			{
				this->TogglePause();
			}

			if (btn == jam::JoystickButton::SELECT)
			{
				this->ReturnToMenu();
			}
		}
	}

	void SceneGame::JoystickMove(int id, int dx, int dy)
	{
		if (id == 0)
		{
			if (dx < 0)
			{
				this->joyLeft = true;
				this->joyRight = false;
			}
			else if (dx > 0)
			{
				this->joyLeft = false;
				this->joyRight = true;
			}
			else
			{
				this->joyLeft = false;
				this->joyRight = false;
			}

			if (dy < 0)
			{
				this->joyUp = true;
				this->joyDown = false;
			}
			else if (dy > 0)
			{
				this->joyUp = false;
				this->joyDown = true;
			}
			else
			{
				this->joyUp = false;
				this->joyDown = false;
			}
		}
	}

	void SceneGame::KeyDown(uint8_t key)
	{
		if (key == jam::key::KEY_A || key == jam::key::KEY_LEFT)
		{
			this->keyLeft = true;
		}
		if (key == jam::key::KEY_D || key == jam::key::KEY_RIGHT)
		{
			this->keyRight = true;
		}
		if (key == jam::key::KEY_W || key == jam::key::KEY_UP)
		{
			this->keyUp = true;
		}
		if (key == jam::key::KEY_S || key == jam::key::KEY_DOWN)
		{
			this->keyDown = true;
		}
		if (key == jam::key::KEY_SPACE || key == jam::key::KEY_ENTER)
		{
			this->keyA = true;
		}
	}

	void SceneGame::KeyUp(uint8_t key)
	{
		if (key == jam::key::KEY_A || key == jam::key::KEY_LEFT)
		{
			this->keyLeft = false;
		}
		if (key == jam::key::KEY_D || key == jam::key::KEY_RIGHT)
		{
			this->keyRight = false;
		}
		if (key == jam::key::KEY_W || key == jam::key::KEY_UP)
		{
			this->keyUp = false;
		}
		if (key == jam::key::KEY_S || key == jam::key::KEY_DOWN)
		{
			this->keyDown = false;
		}
		if (key == jam::key::KEY_SPACE || key == jam::key::KEY_ENTER)
		{
			this->keyA = false;
		}
		if (key == jam::key::KEY_P)
		{
			this->TogglePause();
		}
		if (key == jam::key::KEY_ESCAPE)
		{
			this->ReturnToMenu();
		}
	}

	void SceneGame::MouseMove(int x, int y)
	{
	}

	void SceneGame::MouseClick(jam::MouseButton button, int x, int y)
	{
	}

	void SceneGame::NextLevel()
	{
	}

	jam::IScene* SceneGame::NextScene()
	{
		return this->nextScene;
	}

	void SceneGame::ReturnToMenu()
	{
		this->nextScene = jam::SceneManager::Instance()->GetScene(game::SCENE_MENU);
		if (this->nextScene != nullptr)
		{
			this->nextScene->Construct(this->screenWidth, this->screenHeight);
		}
	}

	
	void SceneGame::TogglePause()
	{
		if (this->gameState == GameState::PAUSE || this->gameState == GameState::PLAY)
		{
			if (this->gameState == GameState::PAUSE)
				this->gameState = GameState::PLAY;
			else
				this->gameState = GameState::PAUSE;		
		}
	}

	void SceneGame::Update(float dt)
	{
		int screenW, screenH;
		screenW = screenH = 0;
		this->GetScreenSize(&screenW, &screenH);
		if (this->gameState == GameState::PAUSE)
			return;

		if (this->gameState == GameState::NEXT_LEVEL || this->gameState == GameState::GAME_OVER)
		{
			this->messageY -= dt * MESSAGE_SPEED;
			if (this->messageY < screenH / 2)
			{
				this->messageY = screenH / 2;
				if (this->gameState == GameState::GAME_OVER)
				{
					this->ReturnToMenu();
				}
				else
				{
					// ZZZ load next level.
					this->gameState = GameState::PLAY;
				}
			}
		}
		this->gameTime += dt;
		this->offset += this->dx * (dt * SCROLL_SPEED);
		if (this->offset < 0.0)
		{
			this->offset = 0.0;
			this->dx = 1;
		}
		else if ((int)this->offset + screenW >= this->tileMap->GetWidth())
		{
			this->offset = this->tileMap->GetWidth() - screenW - 1;
			this->dx = -1;
		}

		float dx, dy;
		if (this->joyUp || this->keyUp)
		{
			dy = -1.0;
		} 
		else if (this->joyDown || this->keyDown)
		{
			dy = 1.0;
		}
		else
		{
			dy = 0.0;
		}

		if (this->joyLeft || this->keyLeft)
		{
			dx = -1.0;
		}
		else if (this->joyRight || this->keyRight)
		{
			dx = 1.0;
		}
		else
		{
			dx = 0.0;
		}
		this->player->SetDirection(dx, dy);
		float x = this->player->GetX();
		float y = this->player->GetY();
		this->player->Update(this, dt);
		if (this->joyA || this->keyA)
		{
			Bullet* bullet = this->player->Shoot();
			if (bullet != nullptr)
			{
				this->playerBullets.push_back(bullet);
			}
		}
		int x1, y1, w1, h1;
		x1 = y1 = w1 = h1 = 0;
		if (this->player->GetPlayerState() == PLAYER_NORMAL)
		{
			this->player->GetHitBox(&x1, &y1, &w1, &h1);
			for (int layerNum = 0; layerNum < this->tileMap->GetLayerCount(); layerNum++)
			{
				jam::TileMapLayer* layer = this->tileMap->GetLayer(layerNum);
				if (layer->GetName() == "tiles")
				{
					int mapX1 = std::ceil((this->offset + x1) / this->tileMap->GetTileWidth());
					int mapX2 = std::floor((this->offset + x1 + w1) / this->tileMap->GetTileWidth());
					int mapY1 = std::ceil(y1 / this->tileMap->GetTileHeight());
					int mapY2 = std::floor((y1 + h1) / this->tileMap->GetTileHeight());
					for (int y = mapY1; y <= mapY2; y++)
					{
						for (int x = mapX1; x <= mapX2; x++)
						{
							if (layer->GetTileID(x, y) != 0)
							{
								// Player has hit a wall tile.
								//std::cout << "Collision at: " << x << ", " << y << std::endl;
								this->SpawnExplosion(this->player->GetX(), this->player->GetY());
								this->player->Hit();
							}
						}
					}

				}

			}
		}

		// Update entities
		jam::Rect rPlayer, rEnemy;
		this->player->GetHitBox(&rPlayer.x, &rPlayer.y, &rPlayer.w, &rPlayer.h);		
		
		for (std::vector<game::Enemy1*>::iterator iter = this->enemies.begin(); iter != this->enemies.end(); iter++)
		{
			(*iter)->GetHitBox(&rEnemy.x, &rEnemy.y, &rEnemy.w, &rEnemy.h);
			if (jam::Collision::Rect_Rect(&rPlayer, &rEnemy))
			{
				this->SpawnExplosion(player->GetX(), player->GetY());
				this->player->Hit();
				this->SpawnExplosion((*iter)->GetX(), (*iter)->GetY());
				(*iter)->Hit();
			}
			(*iter)->Update(this, dt);
		}

		for (std::vector<game::Bullet*>::iterator iter = this->playerBullets.begin(); iter != this->playerBullets.end(); iter++)
		{
			(*iter)->Update(this, dt);
		}

		for (std::vector<game::Bullet*>::iterator iter = this->enemyBullets.begin(); iter != this->enemyBullets.end(); iter++)
		{
			(*iter)->Update(this, dt);
		}

		for (std::vector<game::Explosion*>::iterator iter = this->explosions.begin(); iter != this->explosions.end(); iter++)
		{
			(*iter)->Update(this, dt);
		}

		// Delete old entities.
		for (int i = this->playerBullets.size() - 1; i >= 0; i--)
		{
			game::Bullet* bullet = this->playerBullets[i];
			if (bullet->IsDeleted())
			{
				this->playerBullets.erase(this->playerBullets.begin() + i);
				delete bullet;
			}
		}

		for (int i = this->enemyBullets.size() - 1; i >= 0; i--)
		{
			game::Bullet* bullet = this->enemyBullets[i];
			if (bullet->IsDeleted())
			{
				this->enemyBullets.erase(this->enemyBullets.begin() + i);
				delete bullet;
			}
		}

		for (int i = this->enemies.size() - 1; i >= 0; i--)
		{
			game::Enemy1* enemy = this->enemies[i];
			if (enemy->IsDeleted())
			{
				this->enemies.erase(this->enemies.begin() + i);
				delete enemy;
			}
		}

		for (int i = this->explosions.size() - 1; i >= 0; i--)
		{
			game::Explosion* explosion = this->explosions[i];
			if (explosion->IsDeleted())
			{
				this->explosions.erase(this->explosions.begin() + i);
				delete explosion;
			}
		}

		if (this->enemies.size() == 0)
		{
			// Spawn new enemy if none left.	
			Enemy1* enemy = new Enemy1();
			int y = 32 + (rand() % screenHeight - 64);
			enemy->Construct(this->enemyConfig, nullptr);
			enemy->SetPosition(screenW, y);
			this->enemies.push_back(enemy);
		}
		if (this->player->IsDeleted() && this->gameState == GameState::PLAY)
		{
			jam::backEnd->ResourceManager()->GetAudio(jam::SOUND_PATH + "lose.wav")->Play();
			this->messageY = screenH;
			this->gameState = GameState::GAME_OVER;
		}
	}

	void SceneGame::SpawnExplosion(int x, int y)
	{
		Explosion* explosion = new Explosion();
		explosion->Construct(this->explosionConfig);
		explosion->SetPosition(x, y);
		this->explosions.push_back(explosion);
		jam::backEnd->ResourceManager()->GetAudio(jam::SOUND_PATH + "explosion.wav")->Play();

	}
}