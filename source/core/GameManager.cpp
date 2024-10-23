#include <GameManager.h>
#include "../debug/debug.h"
#include <raylib.h>
#include <raygui.h>
#include <map>
#include <string>
#include <vector>
#include <GameObject.h>
#include <GameOverlays.h>
#include "ScriptCallbacks.h"
#include <StandardInput.h>
#include <GamePad.h>
#include <RenderSystem.h>
extern std::map<std::string, std::string> RES_Textures;
extern std::map<std::string, std::string> RES_Fonts;
RenderSystem *renderSystem;
Font guiFont;
    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)
Texture2D texture;        // Texture loading
const int screenWidth = 1280;
const int screenHeight = 720;
GamePads gamePads;
StandardController controller;

//MenuController menuController;
GameManager::GameManager(bool running) : _running{running}
{

}

GameManager::~GameManager()
{
      UnloadTexture(texture); 
}
GameManager& GameManager::getGameManager()
{
    static GameManager *gameManager = nullptr;
    if(gameManager == nullptr)
    {

        gameManager = new GameManager(true);
        InitWindow(screenWidth, screenHeight, "raylib [textures] example - texture loading and drawing");
        if (!IsWindowReady())
        {
            debugLog("\x1b[16;25HError Creating Window!");
        }
        SetTargetFPS(60);
        renderSystem = &RenderSystem::getRenderSystem();
        //set up controller
        controller = StandardController(&renderSystem->mainCamera);
        gamePads.Init(&controller);
        guiFont = LoadFont(RES_Fonts["DEFAULT"].c_str());//set var for game fonts
      //  texture = LoadTexture(RES_Textures["ENGINE_LOGO"].c_str());  //texture test
        debugLog("Made Game Manager!");
    }
    return *gameManager;
}

void GameManager::destroyGameManager()
{
        GameManager *gameManager = &getGameManager();

        if(gameManager != nullptr)
        {
            debugLog("\x1b[16;20HDestroyed GameManager!");
            delete gameManager;

        }
    }

void GameManager::runGameLoop()
{
   // UpdateCamera(&mainCamera, CAMERA_FREE);
   controller.UpdateInputs();
}
 

bool GameManager::Running(){
     return _running;
}

