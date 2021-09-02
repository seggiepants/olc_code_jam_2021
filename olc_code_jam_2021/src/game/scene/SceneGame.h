#pragma once
#ifndef __SCENE_GAME_H__
#define __SCENE_GAME_H__
#include <vector>
#include "../../jam/IAudio.h"
#include "../../jam/IScene.h"
#include "../../jam/Point2D.h"
#include "../../jam/TileMap.h"
#include "../entity/Player.h"
#include "../entity/Enemy1.h"

namespace game
{

    enum class GameState
    {
        PLAY = 0
        , PAUSE
        , NEXT_LEVEL
        , GAME_OVER
    };

    class SceneGame :
        public jam::IScene
    {
    public:
        SceneGame();
        ~SceneGame();
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
        void NextLevel();
        jam::IScene* NextScene();
        void ReturnToMenu();
        void TogglePause();
        void Update(float dt);
    protected:
        void ClearObjects();
        int screenWidth;
        int screenHeight;
        float gameTime;
        IScene* nextScene;
        bool joyA, joyUp, joyDown, joyLeft, joyRight;
        bool keyA, keyUp, keyDown, keyLeft, keyRight;
        GameState gameState;
        jam::TileMap* tileMap;
        int dx;
        float offset;
        game::Player* player;
        game::Enemy1* enemy;
    };

}
#endif