#pragma once

#ifndef __SCENE_MENU_H__
#define __SCENE_MENU_H__
#include <string>
#include <vector>
#include "../../jam/IAudio.h"
#include "../../jam/IScene.h"
#include "../../jam/JoystickButton.h"
#include "../../jam/Rect.h"

namespace game
{
    class SceneMenu : public jam::IScene //jam::IScene
    {
    public:
        SceneMenu();
        ~SceneMenu();
        void Construct(int screenWidth, int screenHeight);
        void Draw(jam::IRenderer* render);
        void GetScreenSize(int* screenWidth, int* screenHeight); 
        void JoystickButtonDown(int id, jam::JoystickButton btn);
        void JoystickButtonUp(int id, jam::JoystickButton btn);
        void JoystickMove(int id, int dx, int dy);
        void KeyDown(uint8_t key);
        void KeyUp(uint8_t key);
        void MouseMove(int x, int y);
        void MouseClick(jam::MouseButton button, int x, int y);
        jam::IScene* NextScene();
        void Update(float dt);
    protected:
        void MenuSelect(std::string menuItem);
        std::vector<std::pair<std::string, jam::Rect>>* menuText;
        int screenWidth;
        int screenHeight;
        int menuIndex;
        IScene* nextScene;
        bool joyA, joyUp, joyDown, oldJoyA, oldJoyUp, oldJoyDown;
        float joyMoveTimeout;
    };
}

#endif