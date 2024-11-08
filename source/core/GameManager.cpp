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
#include <RenderSystem.h>
extern GameManager *gameManager;
RenderSystem *renderSystem;
StandardController controller;
Font gameFont, newGuiFont;
EngineCallBacks *engineCallBacks;
std::vector<GameObject *> *GameObjects;

GameManager::GameManager(bool running) : _running{running}
{
    GameObjects = new std::vector<GameObject *>();
    engineCallBacks = new EngineCallBacks();
}

GameManager::~GameManager()
{
    
        UnloadFont(gameFont);
        UnloadFont(newGuiFont);
}
void GameManager::Init()
{
        InitWindow(gameConfig.screenWidth, gameConfig.screenHeight, "GodRays NX GAME"); 
        if (!IsWindowReady())
        {
            debugLog("\x1b[16;25HError Creating Window!");
        }
        SetTargetFPS(60);
        renderSystem = &RenderSystem::getRenderSystem();
        renderSystem->skyBoxObj = SkyBoxObject::InstantiateGameObject<SkyBoxObject>(Vector3{0,0,0}, Quaternion{0,0,0,0}, 100.0f);
        //set up controller
        controller = StandardController(&renderSystem->mainCamera);
        gamePads.Init(&controller);
        gameFont = LoadFont(RES_Fonts[_RES::GameFonts::DEFAULT].c_str());
        newGuiFont = LoadFont(RES_Fonts[_RES::GameFonts::GUI].c_str());
        GuiSetFont(newGuiFont);
        if(IsFontReady(gameFont))
            debugLog("Made Font!");
        debugLog("Made Game Manager!");
}

void GameManager::destroyGameManager()
{
    
        if(gameManager != nullptr)
        {
            debugLog("\x1b[16;20HDestroyed GameManager!");
            delete gameManager;

        }
}

void GameManager::runGameLoop()
{
    controller.UpdateInputs();
    engineCallBacks->PhysicsUpdate();
    engineCallBacks->RunUpdateCallbacks();
}
 

bool GameManager::Running(){
     return _running;
}

