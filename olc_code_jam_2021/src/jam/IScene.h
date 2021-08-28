#pragma once
#ifndef __ISCENE_H__
#define __ISCENE_H__

#include <cstdint>
#include "JoystickButton.h"
#include "KeyCodes.h"
#include "MouseButton.h"
#include "IRenderer.h"

namespace jam
{
    class IScene
    {
    public:
        virtual ~IScene() {};
        virtual void Construct(int screenWidth, int screenHeight) = 0;
        virtual void Draw(jam::IRenderer*) = 0;
        virtual void GetScreenSize(int* screenWidth, int* screenHeight) = 0;
        virtual void Update(float dt) = 0;
        virtual void JoystickButtonDown(int id, JoystickButton btn) = 0;
        virtual void JoystickButtonUp(int id, JoystickButton btn) = 0;
        virtual void JoystickMove(int id, int dx, int dy) = 0;
        virtual void KeyDown(uint8_t key) {};
        virtual void KeyUp(uint8_t key) {};
        virtual void MouseMove(int x, int y) = 0;
        virtual void MouseClick(MouseButton button, int x, int y) = 0;
        virtual IScene* NextScene() = 0;
    };
}
#endif

