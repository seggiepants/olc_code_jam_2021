#include <cstdlib>
#include <time.h>
#include "../jam/Shared.h"

#ifdef BACKEND_PGE
#include "../jam/backend/pge/BackendPGE.h"
#endif
#ifdef BACKEND_SDL2
#include "../jam/backend/sdl2/BackendSDL2.h"
#endif

#include "../jam/SceneManager.h"
#include "../game/scene/SceneGame.h"
#include "../game/scene/SceneMenu.h"
#include "../jam/BitmapFont.h"
#include "GameAssets.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

    int main(int argc, char** argv)
    {
#ifdef BACKEND_PGE
        jam::BackendPGE game;
#endif
#ifdef BACKEND_SDL2
        jam::BackendSDL2 game;
#endif
        jam::backEnd = &game;
        std::srand((unsigned int)time(nullptr));

        if (game.Construct("OLC Code Jam 2021", SCREEN_WIDTH, SCREEN_HEIGHT))
        {
            jam::BitmapFont* kenneyFuture14 = new jam::BitmapFont();
            kenneyFuture14->Construct(game::FONT_SMALL_PATH);
            jam::backEnd->ResourceManager()->PreloadFont(game::FONT_SMALL, kenneyFuture14);

            jam::BitmapFont* kenneyFuture20 = new jam::BitmapFont();
            kenneyFuture20->Construct(game::FONT_LARGE_PATH);
            jam::backEnd->ResourceManager()->PreloadFont(game::FONT_LARGE, kenneyFuture20);

            game::SceneGame* sceneGame = new game::SceneGame();
            game::SceneMenu* sceneMenu = new game::SceneMenu();
            jam::SceneManager::Instance()->AddScene(game::SCENE_MENU, sceneMenu);
            jam::SceneManager::Instance()->AddScene(game::SCENE_GAME, sceneGame);
            jam::SceneManager::Instance()->AddScene(game::SCENE_EXIT, nullptr);

            sceneMenu->Construct(SCREEN_WIDTH, SCREEN_HEIGHT);
            game.Start(jam::SceneManager::Instance()->GetScene(game::SCENE_MENU)); 
        }
        return 0;
    }
