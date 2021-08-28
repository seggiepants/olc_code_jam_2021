#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>
#include <iostream>
#include "../../jam/IFont.h"
#include "../GameAssets.h"
#include "SceneGame.h"
#include "../../jam/SceneManager.h"
#include "SceneMenu.h"
#include "../../jam/Shared.h"
#include "../GameAssets.h"

namespace game
{	
	#define JOY_MOVE_DELAY 0.2

	SceneMenu::SceneMenu()  
	{		
		this->screenWidth = this->screenHeight = 0;
		this->menuText = new std::vector<std::pair<std::string, jam::Rect>>();
		this->nextScene = nullptr;
		this->joyA = this->oldJoyA = false;
		this->joyUp = this->oldJoyUp = false;
		this->joyDown = this->oldJoyDown = false;
		this->joyMoveTimeout = 0;		
	}

	SceneMenu::~SceneMenu() 
	{
		this->menuText->clear();
		delete this->menuText;
		this->menuText = nullptr;

	}

	void SceneMenu::Construct(int screenWidth, int screenHeight)
	{
		const int numStars = 4000;
		this->menuText->clear();
		this->menuText->push_back(std::pair<std::string, jam::Rect>("PLAY", { 0, 0, 0, 0 }));
#ifndef KIOSK_MODE
		this->menuText->push_back(std::pair<std::string, jam::Rect>("EXIT", { 0, 0, 0, 0 }));
#endif

		this->menuIndex = 0;
		this->nextScene = (IScene*)this;
	}

	void SceneMenu::Draw(jam::IRenderer* render)
	{
		jam::rgb bg(0, 0, 0, 255);
		render->Clear(bg);
		int width, height;
		jam::rgb color;

		render->GetScreenSize(&width, &height);
		float halfW, halfH;
		jam::IFont* fontLarge = jam::backEnd->ResourceManager()->GetFont(game::FONT_LARGE);
		jam::IFont* fontSmall = jam::backEnd->ResourceManager()->GetFont(game::FONT_SMALL);
		render->GetScreenSize(&this->screenWidth, &this->screenHeight);
		halfW = this->screenWidth / 2.0;
		halfH = this->screenHeight / 2.0;
		color.r = color.g = color.b = color.a = 255;
		std::string msg = "OLC CODE JAM";
		fontLarge->MeasureText(msg, &width, &height);
		float sx, sy;
		sx = (float)((this->screenWidth - width) / 2);
		sy = 32 + height;
		fontLarge->DrawText(render, msg, (int)sx, (int)sy, color);
		msg = "2021";
		fontLarge->MeasureText(msg, &width, &height);
		sx = (float)((this->screenWidth - width) / 2);
		sy += height;
		fontLarge->DrawText(render, msg, (int)sx, (int)sy, color);
		// Find the biggest.
		sy += height * 2;
		int maxWidth = 0 , maxHeight = 0;
		for (std::pair<std::string, jam::Rect> menu : *this->menuText)
		{
			fontSmall->MeasureText(menu.first, &width, &height);
			maxWidth = width > maxWidth ? width : maxWidth;
			maxHeight = height > maxHeight ? height: maxHeight;
		}
		sx = (float)((this->screenWidth - maxWidth) / 2);
		int border = 4;
		int idx = 0;
		jam::rgb black;
		black.r = black.g = black.b = 0;
		black.a = 255;
		jam::rgb highlight;
		highlight.r = highlight.g = 0;
		highlight.b = 128;
		highlight.a = 255;
		jam::rgb white;
		white.r = white.g = white.b = white.a = 255;
		for (std::vector<std::pair<std::string, jam::Rect>>::iterator menu = this->menuText->begin(); menu != this->menuText->end(); menu++)
		{
			menu->second.x = sx - border;
			menu->second.y = sy - border;
			menu->second.w = maxWidth + (2 * border);
			menu->second.h = maxHeight + (2 * border);
			if (idx == this->menuIndex)
			{
				render->FillRect(menu->second.x, menu->second.y, menu->second.x + menu->second.w, menu->second.y + menu->second.h, highlight);
				fontSmall->DrawText(render, menu->first, sx, sy + maxHeight, white);
			}
			else
			{
				render->FillRect(menu->second.x, menu->second.y, menu->second.x + menu->second.w, menu->second.y + menu->second.h, black);
				fontSmall->DrawText(render, menu->first, sx, sy + maxHeight, white);
			}
			sy += maxHeight + border * 3; // two for this item + one for the top of the next.
			idx++;
		}
	}

	void SceneMenu::GetScreenSize(int* screenWidth, int* screenHeight) 
	{ 
		*screenWidth = this->screenWidth; 
		*screenHeight = this->screenHeight; 
	}

	void SceneMenu::JoystickButtonDown(int id, jam::JoystickButton btn)
	{
		if (id == 0)
		{
			if (btn == jam::JoystickButton::A)
			{
				this->oldJoyA = this->joyA;
				this->joyA = true;
			}
			else if (btn == jam::JoystickButton::DPAD_UP)
			{
				this->oldJoyUp = this->joyUp;
				this->joyUp = true;
			}
			else if (btn == jam::JoystickButton::DPAD_DOWN)
			{
				this->oldJoyDown = this->joyDown;
				this->joyDown = true;
			}
		}
	}

	void SceneMenu::JoystickButtonUp(int id, jam::JoystickButton btn)
	{
		if (id == 0)
		{
			if (btn == jam::JoystickButton::A)
			{
				this->oldJoyA = this->joyA;
				this->joyA = false;
				if (this->oldJoyA != this->joyA)
				{
					this->MenuSelect(this->menuText->at(this->menuIndex).first);
				}
			}
			else if (btn == jam::JoystickButton::DPAD_UP)
			{
				this->oldJoyUp = this->joyUp;
				this->joyUp = false;
				if (this->oldJoyUp != this->joyUp)
				{
					this->menuIndex--;
					if (this->menuIndex < 0)
						this->menuIndex = 0;
				}
			}
			else if (btn == jam::JoystickButton::DPAD_DOWN)
			{
				this->oldJoyDown = this->joyDown;
				this->joyDown = false;
				if (this->oldJoyDown != this->joyDown)
				{
					this->menuIndex++;
					if (this->menuIndex >= this->menuText->size())
						this->menuIndex = this->menuText->size() - 1;
				}
			}

		}
		
	}

	void SceneMenu::JoystickMove(int id, int dx, int dy)
	{
		if (this->joyMoveTimeout <= 0.0)
		{
			if (dy != 0)
			{
				if (dy < 0)
				{
					this->menuIndex--;
					if (this->menuIndex < 0)
						this->menuIndex = 0;
				}
				else if (dy > 0)
				{
					this->menuIndex++;
					if (this->menuIndex >= this->menuText->size())
						this->menuIndex = this->menuText->size() - 1;
				}
				this->joyMoveTimeout = JOY_MOVE_DELAY;
			}
		}
	}

	void SceneMenu::KeyDown(uint8_t key)
	{
		// Nothing yet
	}

	void SceneMenu::KeyUp(uint8_t key)
	{
		if (key == jam::key::KEY_UP)
		{
			this->menuIndex--;
			if (this->menuIndex < 0)
				this->menuIndex = 0;

		}
		else if (key == jam::key::KEY_DOWN)
		{
			this->menuIndex++;
			if (this->menuIndex >= this->menuText->size())
			{
				this->menuIndex = this->menuText->size() - 1;
			}
		}
		else if (key == jam::key::KEY_ENTER || key == jam::key::KEY_SPACE || key == jam::key::KEY_RETURN)
		{
			std::string menuItem = this->menuText->at(this->menuIndex).first;
			this->MenuSelect(menuItem);
		}
	}

	void SceneMenu::MenuSelect(std::string menuItem)
	{
		if (menuItem == "PLAY")
		{
			this->nextScene = jam::SceneManager::Instance()->GetScene(game::SCENE_GAME);
			if (this->nextScene != nullptr)
				this->nextScene->Construct(this->screenWidth, this->screenHeight);
		}
#ifndef KIOSK_MODE
		else if (menuItem == "EXIT")
		{
			this->nextScene = jam::SceneManager::Instance()->GetScene(game::SCENE_EXIT);
			if (this->nextScene != nullptr)
				this->nextScene->Construct(this->screenWidth, this->screenHeight);
		}
#endif
	}

	void SceneMenu::MouseMove(int x, int y)
	{

	}

	void SceneMenu::MouseClick(jam::MouseButton button, int x, int y)
	{
		if (button == jam::MouseButton::LEFT)
		{
			int index = 0;
			for(std::vector<std::pair<std::string, jam::Rect>>::iterator item = this->menuText->begin(); item != this->menuText->end(); item++)
			{
				jam::Rect r = (*item).second;
				if (x > r.x && x <= (r.x + r.w) && y >= r.y && y <= (r.y + r.h))
				{
					this->menuIndex = index;
					this->MenuSelect((*item).first);
				}
				index++;
			}
		}

	}

	jam::IScene* SceneMenu::NextScene()
	{
		return this->nextScene;
	}
	
	void SceneMenu::Update(float dt)
	{
		const float starSpeed = 10.0;
		float x, y, z;
		if (this->joyMoveTimeout > 0)
		{
			this->joyMoveTimeout -= dt;
			if (this->joyMoveTimeout < 0.0)
				this->joyMoveTimeout = 0.0;
		}
	}
}