/*
/Things



custom z-lib compression for game data


-Text only menus for now-
selction options
change text color when hilighted
--

-Demo Levels
Plane and Sphere

add more camera types to be used in game
view frustrum

*/


/*
Todo:
----------------------------
*DONE
___________________________

Implement Game States - Done
-------------------------
*WIP
___________________________
**Current
Loading Scene System - WIP
Particle system (Dynamic Batched Software Particles)- Breaks RenderText, May need its own Camera(Text/UI)
Video system (MP4 Playback)- MPV -
---------------------------
*NOT STARTED
__________________________

----------------------------
*FUTURE IMPLEMENTATIONS
_____________________________
Skeletal Animation -
IK -
Procedural Animation -

*/





#include <switch.h>
#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include "debug/debug.h"
#include <GameManager.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <switch.h>
#include <ScriptCallbacks.h>
#include <GameStates.h>
#include <map>

extern std::map<std::string, std::string> RES_Fonts;
extern std::map<std::string, std::string> RES_Textures;
extern std::map<std::string, std::string> RES_Models;

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

void Test()
{
    //test gameobject
    GameObject* test = GameObject::InstantiateGameObject<GameObject>(Vector3{0,0,0}, Quaternion{0,0,0,1}, Vector3{1,1,1},  LoadModel(RES_Models["ROBOT"].c_str()));
}

void EngineMain()
{
    debugLog("Engine Starting...");
      //init GM
    gameManager = &GameManager::getGameManager();
    //TEST
    Test();
    debugLog("Living Objects Count: %d", GameObjects->size());
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