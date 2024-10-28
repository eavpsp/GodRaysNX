#include <GameManager.h>
#include "../debug/debug.h"
#include <raylib.h>
#include <map>
#include <string>
#include <vector>
#include <GameObject.h>
#include <GameOverlays.h>
#include "ScriptCallbacks.h"
#include <StandardInput.h>
#include <GamePad.h>
#include <RenderSystem.h>
RenderSystem *renderSystem;
Font gameFont, newGuiFont;
const int screenWidth = 1280;
const int screenHeight = 720;
GamePads gamePads;
StandardController controller;
extern std::map<int, std::string> RES_Fonts;
GameManager::GameManager(bool running) : _running{running}
{

}

GameManager::~GameManager()
{
      UnloadFont(gameFont);
}
GameManager& GameManager::getGameManager()
{
    static GameManager *gameManager = nullptr;
    if(gameManager == nullptr)
    {

        gameManager = new GameManager(true);
        InitWindow(screenWidth, screenHeight, "GodRays NX GAME"); 
        if (!IsWindowReady())
        {
            debugLog("\x1b[16;25HError Creating Window!");
        }
        SetTargetFPS(60);
        renderSystem = &RenderSystem::getRenderSystem();
        //set up controller
        controller = StandardController(&renderSystem->mainCamera);
        gamePads.Init(&controller);
        gameFont = LoadFont(RES_Fonts[_RES::GameFonts::DEFAULT].c_str());//set var for game fonts
        newGuiFont = LoadFont(RES_Fonts[_RES::GameFonts::GUI].c_str());//set var for game fonts
        GuiSetFont(newGuiFont);
        if(IsFontReady(gameFont))
            debugLog("Made Font!");
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
   controller.UpdateInputs();
}
 

bool GameManager::Running(){
     return _running;
}

