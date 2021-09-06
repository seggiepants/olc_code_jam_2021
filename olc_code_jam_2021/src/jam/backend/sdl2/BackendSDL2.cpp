#include "BackendSDL2.h"
#include <iostream>
#include "RendererSDL2.h"
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "KeyCodesSDL2.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
namespace jam
{
    BackendSDL2::BackendSDL2()
    {
        this->currentScene = nullptr;
        this->window = nullptr;
        this->renderer = nullptr;
        this->mouseX = this->mouseY = 0;
        this->mouseBtnLeft = this->mouseBtnRight = this->oldMouseBtnLeft = this->oldMouseBtnRight = false;
        key::KeyInit_SDL2();        
    }

    BackendSDL2::~BackendSDL2()
    {

        IMG_Quit();

        Mix_CloseAudio();
        Mix_Quit();

        TTF_Quit();

        if (this->render != nullptr)
        {
            delete this->render;
            this->render = nullptr;
        }

        if (this->renderer != nullptr)
        {
            SDL_DestroyRenderer(this->renderer);
            this->renderer = nullptr;
        }

        if (this->window != nullptr)
        {     
            SDL_DestroyWindow(this->window);
            this->window = nullptr;
        }

        //Quit SDL subsystems
        SDL_Quit();
    }

    void BackendSDL2::CloseJoysticks()
    {
        for (std::vector<JoystickInfoSDL*>::iterator iter = this->joysticks.begin(); iter != this->joysticks.end(); iter++)
        {
            if ((*iter)->isGamePad)
            {
                SDL_GameControllerClose((*iter)->gamepad);
            }
            else
            {
                SDL_JoystickClose((*iter)->joystick);
            }
            delete (*iter);
            (*iter) = nullptr;
        }
        this->joysticks.clear();
    }

    bool BackendSDL2::Construct(std::string title, int screenWidth, int screenHeight)
    {
		//Initialization flag
		bool success = true;

		//Initialize SDL
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
		{
			std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
			success = false;
		}
		else
		{
			//Create window
			this->window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
			if (this->window == NULL)
			{
				std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
				success = false;
			}
			else
			{
				this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
				SDL_SetRenderDrawColor(this->renderer, 255, 255, 255, 255);

				/* Clear the entire screen to our selected color. */
				SDL_RenderClear(this->renderer);
			}

            if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
            {
                std::cerr << "Image loading coult not be initialized: \"" << SDL_GetError() << "\"" << std::endl;
                success = false;
            }

			if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) < 0) {
				std::cerr << "Sound mixer could not be initialized: \"" << SDL_GetError() << "\"" << std::endl;
				success = false;
			}

			if (Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 512) < 0) {
				std::cerr << "Sound mixer could not be opened: \"" << SDL_GetError() << "\"" << std::endl;
				success = false;
			}


			if (TTF_Init() < 0) {
				std::cerr << "Font system could not be initialized : \"" << TTF_GetError() << "\"" << std::endl;
				success = false;
			}

            this->OpenJoysticks();
		}

        this->render = nullptr;        

		return success;
    }

    bool BackendSDL2::IsKeyDown(uint8_t code)
    {
        return this->Key[code] && !this->oldKey[code];
    }

    bool BackendSDL2::IsKeyPressed(uint8_t code)
    {
        return this->Key[code];
    }

    bool BackendSDL2::IsKeyReleased(uint8_t code)
    {
        return !this->Key[code] && this->oldKey[code];
    }

    int BackendSDL2::JoystickButtonToSDL(JoystickButton btn)
    {
        switch (btn)
        {
        case JoystickButton::A:
            return SDL_CONTROLLER_BUTTON_A;
        case JoystickButton::B:
            return SDL_CONTROLLER_BUTTON_B;
        case JoystickButton::DPAD_DOWN:
            return SDL_CONTROLLER_BUTTON_DPAD_DOWN;
        case JoystickButton::DPAD_LEFT:
            return SDL_CONTROLLER_BUTTON_DPAD_LEFT;
        case JoystickButton::DPAD_RIGHT:
            return SDL_CONTROLLER_BUTTON_DPAD_RIGHT;
        case JoystickButton::DPAD_UP:
            return SDL_CONTROLLER_BUTTON_DPAD_UP;
        case JoystickButton::LEFT_SHOULDER:
            return SDL_CONTROLLER_BUTTON_LEFTSHOULDER;
        case JoystickButton::RIGHT_SHOULDER:
            return SDL_CONTROLLER_BUTTON_RIGHTSHOULDER;
        case JoystickButton::SELECT:
            return SDL_CONTROLLER_BUTTON_BACK;
        case JoystickButton::START:
            return SDL_CONTROLLER_BUTTON_START;
        case JoystickButton::X:
            return SDL_CONTROLLER_BUTTON_X;
        case JoystickButton::Y:
            return SDL_CONTROLLER_BUTTON_Y;
        default:
            return (int)btn;
        }
    }

    JoystickButton BackendSDL2::SDLToJoystickButton(int btn)
    {
        switch (btn)
        {
        case SDL_CONTROLLER_BUTTON_A:
            return JoystickButton::A;
        case SDL_CONTROLLER_BUTTON_B:
            return JoystickButton::B;
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
            return JoystickButton::DPAD_DOWN;
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
            return JoystickButton::DPAD_LEFT;
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
            return JoystickButton::DPAD_RIGHT;
        case SDL_CONTROLLER_BUTTON_DPAD_UP:
            return JoystickButton::DPAD_UP;
        case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
            return JoystickButton::LEFT_SHOULDER;
        case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
            return JoystickButton::RIGHT_SHOULDER;
        case SDL_CONTROLLER_BUTTON_BACK:
            return JoystickButton::SELECT;
        case SDL_CONTROLLER_BUTTON_START:
            return JoystickButton::START;
        case SDL_CONTROLLER_BUTTON_X:
            return JoystickButton::X;
        case SDL_CONTROLLER_BUTTON_Y:
            return JoystickButton::Y;
        default:
            return JoystickButton::UNKNOWN;
        }
     }


    void BackendSDL2::OpenJoysticks()
    {
        this->CloseJoysticks();
        for (int i = 0; i < SDL_NumJoysticks(); i++)
        {
            JoystickInfoSDL* info = new JoystickInfoSDL();
            info->id = i;
            info->isGamePad = SDL_IsGameController(i);
            if (info->isGamePad)
            {
                info->joystick = nullptr;
                info->gamepad = SDL_GameControllerOpen(i);
                const char* name = SDL_GameControllerNameForIndex(i);
                std::cout << "Found Joystick #" << i + 1 << ": \"" << name << "\"" << std::endl;
            }
            else
            {
                info->joystick = SDL_JoystickOpen(i);
                info->gamepad = nullptr;
            }
            this->joysticks.push_back(info);
        }
    }

#ifdef __EMSCRIPTEN__
    void BackendSDL2::emscripten_update(void* param) 
    {
        BackendSDL2* obj = (BackendSDL2*)param;
        if (obj->currentScene != nullptr)
        {
            obj->Step();
        }
        else
        {
            emscripten_cancel_main_loop();
        }
    }
#endif

    void BackendSDL2::Step()
    {
        previous = current;
        current = SDL_GetPerformanceCounter();
        Uint64 ticks = current - previous;
        float dt = (float)ticks / (float)SDL_GetPerformanceFrequency();
        SDL_Event e;
        for (int i = 0; i < MAX_KEYS; i++)
        {
            this->oldKey[i] = this->Key[i];
        }
        //this->currentScene->JoystickMove(0, 0, 0);
        while (SDL_PollEvent(&e))
        {
            //User requests quit
            if (e.type == SDL_QUIT)
            {
                this->currentScene = nullptr;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                this->Key[e.key.keysym.scancode] = true;
                if (this->Key[e.key.keysym.scancode] != this->oldKey[e.key.keysym.scancode])
                {
                    if (this->currentScene != nullptr)
                    {
                        this->currentScene->KeyDown(e.key.keysym.scancode);
                    }
                }

            }
            else if (e.type == SDL_KEYUP)
            {
                this->Key[e.key.keysym.scancode] = false;
                if (this->Key[e.key.keysym.scancode] != this->oldKey[e.key.keysym.scancode])
                {
                    if (this->currentScene != nullptr)
                    {
                        this->currentScene->KeyUp(e.key.keysym.scancode);
                    }
                }
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                switch (e.button.button)
                {
                case SDL_BUTTON_LEFT:
                    this->oldMouseBtnLeft = this->mouseBtnLeft;
                    this->mouseBtnLeft = true;
                    break;
                case SDL_BUTTON_RIGHT:
                    this->oldMouseBtnRight = this->mouseBtnRight;
                    this->mouseBtnRight = true;
                    break;
                }
            }
            else if (e.type == SDL_MOUSEBUTTONUP)
            {
                switch (e.button.button)
                {
                case SDL_BUTTON_LEFT:
                    this->oldMouseBtnLeft = this->mouseBtnLeft;
                    this->mouseBtnLeft = false;
                    if (this->oldMouseBtnLeft)
                    {
                        if (this->currentScene != nullptr)
                        {
                            this->currentScene->MouseClick(jam::MouseButton::LEFT, this->mouseX, this->mouseY);
                        }
                    }
                    break;
                case SDL_BUTTON_RIGHT:
                    this->oldMouseBtnRight = this->mouseBtnRight;
                    this->mouseBtnRight = false;
                    if (this->oldMouseBtnRight)
                    {
                        if (this->currentScene != nullptr)
                        {
                            this->currentScene->MouseClick(jam::MouseButton::RIGHT, this->mouseX, this->mouseY);
                        }
                    }
                    break;
                }
            }
            else if (e.type == SDL_MOUSEMOTION)
            {
                this->mouseX = e.motion.x;
                this->mouseY = e.motion.y;
            }
            else if (e.type == SDL_JOYHATMOTION)
            {
                // Dpad
                int id = e.jhat.which;
                if (e.jhat.hat == 0)
                {
                    const int BIT_UP = 1;
                    const int BIT_RIGHT = 2;
                    const int BIT_DOWN = 4;
                    const int BIT_LEFT = 8;
                    int dx, dy;
                    dx = dy = 0;
                    if (e.jhat.value & BIT_LEFT)
                        dx = -1;
                    else if (e.jhat.value & BIT_RIGHT)
                        dx = 1;

                    if (e.jhat.value & BIT_UP)
                        dy = -1;
                    else if (e.jhat.value & BIT_DOWN)
                        dy = 1;

                    if (this->currentScene != nullptr)
                    {
                        this->currentScene->JoystickMove(id, dx, dy);
                    }

                }
            }
            else if (e.type == SDL_JOYAXISMOTION)
            {
                const int JOYSTICK_DEAD_ZONE = 8000;
                int id = e.jaxis.which;
                int dx, dy;
                dx = dy = 0;
                if (e.jaxis.axis == 0) // Left/Right
                {
                    if (e.jaxis.value < -1 * JOYSTICK_DEAD_ZONE)
                    {
                        dx = -1;
                    }
                    else if (e.jaxis.value > JOYSTICK_DEAD_ZONE)
                    {
                        dx = 1;
                    }
                }
                if (e.jaxis.axis == 1) // Up/Down
                {
                    if (e.jaxis.value < -1 * JOYSTICK_DEAD_ZONE)
                    {
                        dy = -1;
                    }
                    else if (e.jaxis.value > JOYSTICK_DEAD_ZONE)
                    {
                        dy = 1;
                    }
                }
                if (this->currentScene != nullptr)
                {
                    this->currentScene->JoystickMove(id, dx, dy);
                }

            }
            else if (e.type == SDL_JOYBUTTONDOWN)
            {
                int id = e.cbutton.which;
                jam::JoystickButton btn = this->SDLToJoystickButton(e.jbutton.button);
                if (this->currentScene != nullptr)
                {
                    this->currentScene->JoystickButtonDown(id, btn);
                }
            }
            else if (e.type == SDL_JOYBUTTONUP)
            {
                int id = e.cbutton.which;
                jam::JoystickButton btn = this->SDLToJoystickButton(e.jbutton.button);
                if (this->currentScene != nullptr)
                {
                    this->currentScene->JoystickButtonUp(id, btn);
                }
            }
        }

        if (this->currentScene != nullptr)
        {
            this->currentScene->Update(dt);
            this->currentScene->Draw(this->render);
            /*
            if (!globals.currentScene->running()) {
                globals.state = globals.currentScene->getNextState();
                NextScene();
            }
            */
            SDL_RenderPresent(this->renderer);
            this->currentScene = this->currentScene->NextScene();
        }
    }

    void BackendSDL2::Start(IScene* scene)
    {       
        bool joyMotion, oldJoyMotion;
        int joyID;
        if (this->render != nullptr)
        {
            delete this->render;
            this->render = nullptr;
        }
        RendererSDL2* render = new RendererSDL2();
        render->Init(this->window, this->renderer);
        this->render = (IRenderer*)render;

        this->currentScene = scene;
        int screenWidth, screenHeight;
        this->render->GetScreenSize(&screenWidth, &screenHeight);
        this->currentScene->Construct(screenWidth, screenHeight);

        current = SDL_GetPerformanceCounter();
        joyMotion = false;
        joyID = false;

#ifdef __EMSCRIPTEN__
        emscripten_set_main_loop_arg(emscripten_update, (void*)this, -1, 1);
#else

        while (this->currentScene != nullptr) {
            this->Step();
        }
#endif
    }

    bool BackendSDL2::OnUserCreate()
    {
        return true;
    }

    bool BackendSDL2::OnUserUpdate(float dt)
    {
        if (this->currentScene != nullptr)
        {
            this->currentScene->Update(dt);
            if (this->render != nullptr)
            {
                this->currentScene->Draw((jam::IRenderer*)this->render);
            }
        }
        return true;
    }

    SDL_Window* BackendSDL2::GetWindow()
    {
        return this->window;
    }

    SDL_Renderer* BackendSDL2::GetRenderer()
    {
        return this->renderer;
    }

}