#pragma once

#ifndef __BACKEND_SDL2_H__
#define __BACKEND_SDL2_H__
#include <SDL2/SDL.h>
#include <vector>
#include "../IBackend.h"
#include "ResourceManagerSDL2.h"

namespace jam
{

    struct JoystickInfoSDL
    {
        int id;
        bool isGamePad;
        SDL_Joystick* joystick;
        SDL_GameController* gamepad;
    };
    class BackendSDL2 :
        public IBackend
    {
    public:
        BackendSDL2();
        ~BackendSDL2();
        bool Construct(std::string title, int screenWidth, int screenHeight) override;
        bool IsKeyDown(uint8_t code);
        bool IsKeyPressed(uint8_t code);
        bool IsKeyReleased(uint8_t code);
        void Start(IScene* scene) override;
        bool OnUserCreate();
        bool OnUserUpdate(float dt);
        IResourceManager* ResourceManager() override { return ResourceManagerSDL2::Instance(); };
        SDL_Window* GetWindow();
        SDL_Renderer* GetRenderer();
    protected:
        void CloseJoysticks();
        void OpenJoysticks();
        int JoystickButtonToSDL(JoystickButton btn);
        void Step();
#ifdef __EMSCRIPTEN__
        static void emscripten_update(void* param);
#endif
        JoystickButton SDLToJoystickButton(int btn);
        SDL_Window* window;
        SDL_Renderer* renderer;
        int mouseX, mouseY;
        bool mouseBtnLeft, mouseBtnRight, oldMouseBtnLeft, oldMouseBtnRight;        
        int numJoysticks;
        std::vector<JoystickInfoSDL*> joysticks;
        Uint64 previous, current;
    };
}
#endif