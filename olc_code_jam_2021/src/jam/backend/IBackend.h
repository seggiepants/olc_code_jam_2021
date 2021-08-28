#pragma once

#ifndef __IBACKEND_H__
#define __IBACKEND_H__

#include <string>
#include "../../3rdParty/json/json.hpp"
#include "../IRenderer.h"
#include "../IScene.h"
#include "../JoystickButton.h"
#include "../KeyCodes.h"
#include "../IResourceManager.h"

namespace jam
{
    class IBackend
    {
    public:
        IBackend();
        virtual ~IBackend() {};
        virtual bool Construct(std::string title, int screenWidth, int screenHeight) = 0;                
        virtual void Start(IScene* scene) = 0;
        virtual bool OnUserCreate() = 0;
        virtual bool OnUserUpdate(float dt) = 0;        
        virtual IResourceManager* ResourceManager() = 0;
    protected:
        IScene* currentScene;
        IRenderer* render;
        bool oldKey[MAX_KEYS];
        bool Key[MAX_KEYS];
    };
}
#endif

