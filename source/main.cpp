#include <switch.h>
#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include "debug/debug.h"
#include <GameManager.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <switch.h>
#include <ScriptCallbacks.h>
#include <GameStates.h>


EngineCallBacks *engineCallBacks;
std::vector<GameObject *> *GameObjects;
std::vector<EngineObject *> *GraphicsObjects;
GameManager *gameManager;

void initSystem()
{   
    romfsInit();
     debugLogInit();
    debugLog("romFS Init");
    debugLog("System Starting...");
    //init callbacks 
    GameObjects = new std::vector<GameObject *>();
    GraphicsObjects = new std::vector<EngineObject *>();
    engineCallBacks = new EngineCallBacks();
    debugLog("Engine Callbacks Init");
    ENGINE_STATES::ChangeState(ENGINE_STATES::IN_GAME);
}



void EngineMain()
{
     debugLog("Engine Starting...");
      //init GM
    gameManager = &GameManager::getGameManager();

    while (!WindowShouldClose() && gameManager->Running())    // Detect window close button or ESC key
    {
        if(ENGINE_STATES::GetState() == ENGINE_STATES::IN_GAME)
        {
            
            gameManager->runGameLoop();
            gameManager->renderLoop();
            //Run Update Callbacks
            engineCallBacks->RunUpdateCallbacks();
            
            // Get and process input
            if (GetGamepadButtonPressed() == GAMEPAD_BUTTON_MIDDLE_LEFT)
                gameManager->destroyGameManager();
        }
    }

}
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    initSystem();

    EngineMain();
    debugLog("Game Stopped....");

    romfsExit();
    CloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    debugLog("Exiting....");
    appletExit();
    return EXIT_SUCCESS;

}