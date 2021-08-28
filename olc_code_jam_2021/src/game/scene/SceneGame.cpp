#define _USE_MATH_DEFINES
#include <cmath>
#include "../../jam/IAudio.h"
#include "../../jam/IImage.h"
#include "../../jam/IResourceManager.h"
#include "../../jam/Rect.h"
#include "SceneGame.h"
#include "../../jam/SceneManager.h"
#include "../../jam/Shared.h"
#include "../GameAssets.h"

namespace game
{
	SceneGame::SceneGame()
	{
		this->nextScene = nullptr;
		this->screenWidth = this->screenHeight = 0;
	}

	SceneGame::~SceneGame()
	{
		this->ClearObjects();
	}

	void SceneGame::ClearObjects()
	{
	}

	void SceneGame::Construct(int screenWidth, int screenHeight)
	{
		this->screenWidth = screenWidth;
		this->screenHeight = screenHeight;
		this->gameTime = 0.0;

		this->nextScene = (IScene*)this;
		
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
		float halfW, halfH;
		jam::IFont* fontSmall = jam::backEnd->ResourceManager()->GetFont(game::FONT_SMALL);
		halfW = this->screenWidth / 2.0;
		halfH = this->screenHeight / 2.0;
		std::string msg = "Press <ESCAPE> to";
		int y;
		fontSmall->MeasureText(msg, &width, &height);
		y = ((screenHeight - height) / 2) - height;
		fontSmall->DrawText(render, msg, (screenWidth - width) / 2, y, fg);
		msg = " return to the menu"; 
		fontSmall->MeasureText(msg, &width, &height);
		y += height * 2;
		fontSmall->DrawText(render, msg, (screenWidth - width) / 2, y, fg);
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
		this->gameTime += dt;
	}
}