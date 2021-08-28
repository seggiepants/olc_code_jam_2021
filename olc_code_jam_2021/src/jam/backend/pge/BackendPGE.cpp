#include <iostream>
#define OLC_PGE_APPLICATION
#define OLC_PGE_GAMEPAD
#define OLC_PGEX_SOUND
#define OLC_PGEX_AUDIOCONVERT
#include "BackendPGE.h"
#include "RendererPGE.h"
#include "KeyCodesPGE.h"

namespace jam
{
    BackendPGE::BackendPGE() : IBackend(), olc::PixelGameEngine()
    {
        this->render = nullptr;
        key::KeyInit_PGE();
        this->oldMousePos.x = this->oldMousePos.y = 0;
    }

    BackendPGE::~BackendPGE()
    {
        olc::SOUND::DestroyAudio();
        if (this->render != nullptr)
        {
            delete this->render;
            this->render = nullptr;
        }
    }

    bool BackendPGE::Construct(std::string title, int screenWidth, int screenHeight)
    {
        olc::rcode ret;
        this->joyMotion = this->oldJoyMotion = false;
        ret = ((olc::PixelGameEngine*)this)->Construct(screenWidth, screenHeight, 1, 1);
        if (ret == olc::rcode::OK)
        {
            this->sAppName = title;
        }
        
    
        if (!olc::SOUND::InitialiseAudio())
            std::cerr << "Unable to Initialize Audio" << std::endl;
        
        return ret == olc::rcode::OK;
    }

    bool BackendPGE::IsKeyDown(uint8_t code)
    {
        return this->Key[code] && !this->oldKey[code];
    }

    bool BackendPGE::IsKeyPressed(uint8_t code)
    {
        return this->Key[code];
    }

    bool BackendPGE::IsKeyReleased(uint8_t code)
    {
        return !this->Key[code] && this->oldKey[code];
    }

    olc::GPButtons BackendPGE::JoystickButtonToPGE(JoystickButton btn)
    {
        switch (btn)
        {
        case JoystickButton::A:
            return olc::GPButtons::FACE_D;
        case JoystickButton::B:
            return olc::GPButtons::FACE_R;
        case JoystickButton::DPAD_DOWN:
            return olc::GPButtons::DPAD_D;
        case JoystickButton::DPAD_LEFT:
            return olc::GPButtons::DPAD_L;
        case JoystickButton::DPAD_RIGHT:
            return olc::GPButtons::DPAD_R;
        case JoystickButton::DPAD_UP:
            return olc::GPButtons::DPAD_U;
        case JoystickButton::LEFT_SHOULDER:
            return olc::GPButtons::L1;
        case JoystickButton::RIGHT_SHOULDER:
            return olc::GPButtons::R1;
        case JoystickButton::SELECT:
            return olc::GPButtons::SELECT;
        case JoystickButton::START:
            return olc::GPButtons::START;
        case JoystickButton::X:
            return olc::GPButtons::FACE_L;
        case JoystickButton::Y:
            return olc::GPButtons::FACE_U;
        default:
            return (olc::GPButtons)-1;
        }
    }

    JoystickButton BackendPGE::PGEToJoystickButton(olc::GPButtons btn)
    {
        switch (btn)
        {
        case olc::GPButtons::FACE_D:
            return JoystickButton::A;
        case olc::GPButtons::FACE_R:
            return JoystickButton::B;
        case olc::GPButtons::DPAD_D:
            return JoystickButton::DPAD_DOWN;
        case olc::GPButtons::DPAD_L:
            return JoystickButton::DPAD_LEFT;
        case olc::GPButtons::DPAD_R:
            return JoystickButton::DPAD_RIGHT;
        case olc::GPButtons::DPAD_U:
            return JoystickButton::DPAD_UP;
        case olc::GPButtons::L1:
            return JoystickButton::LEFT_SHOULDER;
        case olc::GPButtons::R1:
            return JoystickButton::RIGHT_SHOULDER;
        case olc::GPButtons::SELECT:
            return JoystickButton::SELECT;
        case olc::GPButtons::START:
            return JoystickButton::START;
        case olc::GPButtons::FACE_L:
            return JoystickButton::X;
        case olc::GPButtons::FACE_U:
            return JoystickButton::Y;
        default:
            return JoystickButton::UNKNOWN;
        }
    }

    void BackendPGE::Start(IScene* scene)
    {
        if (this->render != nullptr)
        {
            delete this->render;
            this->render = nullptr;
        }
        RendererPGE* pgeRender = new RendererPGE();
        pgeRender->Init(this);
        this->render = (IRenderer*)pgeRender;

        this->currentScene = scene;
        int screenWidth, screenHeight;
        this->render->GetScreenSize(&screenWidth, &screenHeight);
        this->currentScene->Construct(screenWidth, screenHeight);
        olc::GamePad::init();
        this->gamePads = olc::GamePad::getGamepads();

        ((olc::PixelGameEngine*)this)->Start();
    }

    bool BackendPGE::OnUserCreate() 
    {
        return true;
    }

    bool BackendPGE::OnUserUpdate(float dt) 
    {
        const float JOYSTICK_DEAD_ZONE = 0.3;
        olc::HWButton btnState;
        for (int i = 0; i < MAX_KEYS; i++)
        {
            this->oldKey[i] = this->Key[i];
            btnState = this->GetKey((olc::Key)i);
            if (this->currentScene != nullptr)
            {
                if (btnState.bReleased)
                {
                    this->currentScene->KeyUp(i);
                }
                if (btnState.bPressed)
                {
                    this->currentScene->KeyDown(i);
                }
            }
            this->Key[i] = btnState.bHeld;
        }

        int id = 0, joyId = 0;
        float lx, ly;
        int dx, dy;
        this->oldJoyMotion = this->joyMotion;
        this->joyMotion = false;
        if (this->currentScene != nullptr)
        {
            if (this->gamePads.size() == 0)
            {
                this->gamePads = olc::GamePad::getGamepads();
            }
            for (std::vector<olc::GamePad*>::iterator iter = this->gamePads.begin(); iter != this->gamePads.end(); iter++)
            {
                lx = ly = 0.0;
                olc::GamePad* joystick = (*iter);
                olc::HWButton btn;
                for (int i = 0; i < JoystickButton::UNKNOWN; i++)
                {
                    btn = joystick->getButton(JoystickButtonToPGE((jam::JoystickButton)i));
                    if (btn.bPressed)
                    {
                        this->currentScene->JoystickButtonDown(id, (jam::JoystickButton)i);
                    }
                    if (btn.bReleased)
                    {
                        this->currentScene->JoystickButtonUp(id, (jam::JoystickButton)i);
                    }
                    lx = joystick->getAxis(olc::GPAxes::LX);
                    ly = joystick->getAxis(olc::GPAxes::LY);
                    dx = dy = 0;
                    if (lx < -1 * JOYSTICK_DEAD_ZONE)
                    {
                        dx = -1;
                    }
                    else if (lx > JOYSTICK_DEAD_ZONE)
                    {
                        dx = 1;
                    }

                    if (ly < -1 * JOYSTICK_DEAD_ZONE)
                    {
                        dy = -1;
                    }
                    else if (ly > JOYSTICK_DEAD_ZONE)
                    {
                        dy = 1;
                    }
                    if (dx != 0 || dy != 0)
                    {
                        this->joyMotion = true;
                        joyId = id;
                        currentScene->JoystickMove(id, dx, dy);
                    }
                }

                id++;
            }
        }

        if (this->joyMotion == false && this->oldJoyMotion == true)
        {
            this->currentScene->JoystickMove(joyId, 0, 0);
        }

        olc::HWButton mouseLeft = this->GetMouse(0);
        olc::HWButton mouseRight = this->GetMouse(1);
        olc::vi2d mousePos = this->GetMousePos();
        if (this->oldMousePos.x != mousePos.x || this->oldMousePos.y != mousePos.y)
        {
            this->oldMousePos.x = mousePos.x;
            this->oldMousePos.y = mousePos.y;
            if (this->currentScene != nullptr)
            {
                this->currentScene->MouseMove(mousePos.x, mousePos.y);
            }
        }

        if (mouseLeft.bReleased && this->currentScene != nullptr)
        {
            this->currentScene->MouseClick(jam::MouseButton::LEFT, mousePos.x, mousePos.y);
        }

        if (mouseRight.bReleased && this->currentScene != nullptr)
        {
            this->currentScene->MouseClick(jam::MouseButton::RIGHT, mousePos.x, mousePos.y);
        }

        if (this->currentScene != nullptr)
        {
            this->currentScene->Update(dt);
            if (this->render != nullptr)
            {
                this->SetPixelMode(olc::Pixel::ALPHA);
                this->currentScene->Draw((jam::IRenderer*)this->render);
                this->SetPixelMode(olc::Pixel::NORMAL);
            }
        }
        this->currentScene = this->currentScene->NextScene();
        return this->currentScene != nullptr;
    }
}