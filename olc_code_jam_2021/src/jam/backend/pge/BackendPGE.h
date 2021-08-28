#ifndef __BACKEND_PGE_H__
#define __BACKEND_PGE_H__
#pragma once
#include <olcPixelGameEngine.h>
#include <olcPGEX_Sound.h>
#include <olcPGEX_AudioConverter.h>
#include <olcPGEX_Gamepad.h>
#include "../IBackend.h"
#include "ResourceManagerPGE.h"

namespace jam
{
    class BackendPGE :
        public IBackend, olc::PixelGameEngine
    {
    public:
        BackendPGE();
        ~BackendPGE();
        bool Construct(std::string title, int screenWidth, int screenHeight) override;
        bool IsKeyDown(uint8_t code);
        bool IsKeyPressed(uint8_t code);
        bool IsKeyReleased(uint8_t code);
        void Start(jam::IScene* scene) override;
        bool OnUserCreate() override;
        bool OnUserUpdate(float dt) override;
        IResourceManager* ResourceManager() override { return ResourceManagerPGE::Instance(); };
    protected:
        void Step();
        olc::GPButtons JoystickButtonToPGE(JoystickButton btn);
        JoystickButton PGEToJoystickButton(olc::GPButtons btn);
        olc::vi2d oldMousePos;
        std::vector<olc::GamePad*> gamePads;
        bool joyMotion, oldJoyMotion;
    };    
}
#endif