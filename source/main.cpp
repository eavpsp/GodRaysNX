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
Video system (MP4 Playback)- FFMPEG - Done
Loading Scene System - Done
-------------------------
*WIP
___________________________
**Current
Physics system - WIP
Input System - WIP
Animation System - WIP
Particle system (Dynamic Batched Software Particles)- WIP
ESC System for Static Objects - WIP
---------------------------
*NOT STARTED
__________________________

----------------------------
*FUTURE IMPLEMENTATIONS
_____________________________
Multithreading -
ComputeShader Pathfinding -
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
#include <map>
#include <VideoPlayer.h>
#include <GameScene.h>
#include <PhysicsComponent.h>
#include <PhysicsWorld.h>
#include <mwmath.h>
extern std::map<std::string, std::string> RES_Fonts;
extern std::map<std::string, std::string> RES_Textures;
extern std::map<int, std::string> RES_Models;
LoadingOverlay *loadingOverlay;
GameSceneManager *sceneManager;
EngineCallBacks *engineCallBacks;
std::vector<GameObject *> *GameObjects;
std::vector<PhysicsComponent *> *PhysicsObjects;
std::vector<EngineObject *> *GraphicsObjects;
GameManager *gameManager;

static float timer = 0;
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
    PhysicsWorld::Init();
    PhysicsObjects = new std::vector<PhysicsComponent *>();
    debugLog("Engine Callbacks Init");
    ENGINE_STATES::ChangeState(ENGINE_STATES::BOOT);
}

void TestVideo()
{
    //test video
   Player::playbackInit("/video/test.mp4");
}   

void TestLoadScene()
{
    //test load scene
    sceneManager = new GameSceneManager();
    GameScene *scene = new GameScene(_RES::Scenes_Types::ROBOT_SCENE);
    sceneManager->LoadScene(scene);//convert to load scene
    
}
void TestPhysics()
{
    //test physics
    //create plane
    //create objects in random positions
    //add physics components to them
    for (size_t i = 0; i < 2; i++)
    {
        /* code */
        Vector3 pos = Vector3{MW_Math::Random(0,3), 20* i, MW_Math::Random(0,3)};
        GameObject *obj = GameObject::InstantiateGameObject<GameObject>(pos, Quaternion{0,0,0,0}, Vector3{1,1,1}, _RES::GetModel(_RES::Model_ID::ROBOT_ID));
        PhysicsComponent *comp = new PhysicsComponent(1000.0f, 2.0f, false);
        obj->AddComponent(comp);
    }
    
}

void BOOT()
{
      BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Welcome to GodRays Game Engine", 420, 260, 20, BLACK);
            DrawText("Booting Up...", 440, 300, 20, BLACK);
    EndDrawing();
    
    timer += GetFrameTime();
    if (timer > 5)
    {
        timer = 0;
        ENGINE_STATES::ChangeState(ENGINE_STATES::IN_GAME);

        TestPhysics();

    }

}
void Wait()
{

    BeginDrawing();
                loadingOverlay->Draw();
    EndDrawing();
    timer += GetFrameTime();
    if (timer > 5)
    {
        timer = 0;
        ENGINE_STATES::ChangeState(ENGINE_STATES::IN_GAME);
    }
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
            PhysicsWorld::Update();
            //Run Update Callbacks
            engineCallBacks->RunUpdateCallbacks();
            
            // Get and process input
            if (GetGamepadButtonPressed() == GAMEPAD_BUTTON_MIDDLE_LEFT)
                gameManager->destroyGameManager();
        }
        else if (ENGINE_STATES::GetState() == ENGINE_STATES::BOOT)
        {
            BOOT();
        }
        else if(ENGINE_STATES::GetState() == ENGINE_STATES::VIDEO)
        {
          
          //Video Playback Here
            if(!Player::playbackLoop())
            {
                ENGINE_STATES::ChangeState(ENGINE_STATES::IN_GAME);
            }
        
        }
        else if(ENGINE_STATES::GetState() == ENGINE_STATES::CUTSCENE)
        {
            //Cutscene Here
            
        }
        else if(ENGINE_STATES::GetState() == ENGINE_STATES::MENU)
        {

        }
        else if (ENGINE_STATES::GetState() == ENGINE_STATES::IDLE)
        {
            Wait();
            
        }
        else if(ENGINE_STATES::GetState() == ENGINE_STATES::LOADING)
        {
            BeginDrawing();
                loadingOverlay->Draw();
            EndDrawing();
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

    debugLogCleanup();
    romfsExit();
    CloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    debugLog("Exiting....");
    appletExit();
    return EXIT_SUCCESS;

}