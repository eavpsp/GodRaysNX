/*
/Random Things


                            cache data in scopes!
Stress Test
Swarm AI
Animations Running all at once
Physics Multiple simulations

Todo:
----------------------------
*DONE
___________________________

Implement Game States - Done
Video system (MP4 Playback)- FFMPEG - Done
Loading Scene System - Done
Physics system - Done
Input System - Done
Animation System - Done
ESC System - Done
View Frustrum - Done
Particle system (Dynamic Batched Software Particles)- Done
Audio Component - Done;
Level Editor - Done
Raycasting - Done
Menu Controller - Implemented Not Tested
    TODO: Default Controller and Main Menu(Scene Selection)
Texture2D component - Mesh Component - Done
Lights - Done
Shaders - Done
Post Processing - Done
Bullet Physics - Done
LOD - Implemented Not Tested
Shadow Maps - Done Thank God
Object Pool - Implemented not tested
Texture2D Animator - Implemented not tested
~SkyBox - Added Not working
~More Overlays - Done
Skeletal Animation - M3D - Already implemented by Raylib
~Add Fog to PPFX - DIDNT WORK
~UI Screen Touch Controls - Tested works on Default
~On Screen Debugger - Implemented
~Split MAIN.cpp to core components - Done
    Engine Procs - used to determine what process should run at what state
~Physics 2D Component - Done
    -set up box or circle and handle collisions
~Update Editor Components For Scene Loading- Done
    `Loader - Done
    `Editor - Done
~Screen Space Interactions/Raycasts - Done
~Animation Events - Done
~Serialize Animation Data - Done
    Add Animation Controller + Data to Res Manager(ControllerID -- AnimationData File To Load)
~Bone Attachments - Done
~Children Objects - Done
Skybox - Done
BP Physics Call backs - Done
Animation Event fix- done
-------------------------
*WIP
___________________________
**Current
~PBR as Shader Option -
~Shader Materials
    Preset Shader Loading with variable input
Change Animation Controller to state machine
ARM SIMD Instruction Set Implementation - 
Add Smart Pointers -
Multithreading -
    Thread Pool-
    Task Scheduler-
    Physics Thread -
    Audio/Video Processing Thread -
    Asset Loading Thread -
        Game Scene Loader -
---------------------------
*NOT STARTED

__________________________
----------------------------
*FUTURE IMPLEMENTATIONS/OPTIMIZATIONS
_____________________________
~File Archive System -
    Zlib Compression and Decompression
IK -
Procedural Animation -
Networking - NET Spirit
    Multiplayer -
*/

//Heap 256MB
//4GB Ram  ~50% used for current levels and preloading levels
#define TOTAL_RAM (4ULL * 1024ULL * 1024ULL * 1024ULL)
#define MAX_LEVEL_RAM (TOTAL_RAM / 3ULL) // 4GB / 3 so 1 third of ram MAX is used for levels
#define MAX_GRB_FILE_SIZE (TOTAL_RAM / 4ULL) // ~1.2 GB //may pre load scenes in ram
#include <switch.h>
#include <raylib.h>
#include "debug/debug.h"
#include <GameManager.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <ScriptCallbacks.h>
#include <map>
#include <VideoPlayer.h>
#include <GameScene.h>
#include <PhysicsComponent.h>
#include <PhysicsWorld.h>
#include <mwmath.h>
#include <Entity.h>
#include <RenderSystem.h>
#include <ResourceManager.h>
#include <EngineProcs.h>

//CORE SYSTEMS
GameManager *gameManager;
EntityComponentSystem ecs;
extern RenderSystem *renderSystem;
PhysicsWorld *physicsWorld;
//CALL BACK VARIABLES

//LOCAL OVERLAYS
BootOverlay *bootOverlay;
LoadingOverlay *loadingOverlay;

//LOCAL VARS
static float timer = 0;

//Get all system procs
extern VIDEO_PROC* videoProc;
extern MENU_PROC* menuProc;
extern IN_GAME_PROC* inGameProc;
extern IDLE_PROC* idleProc;
extern LOADING_PROC* loadingProc;
extern BOOT_PROC* bootProc;
extern TEST_PROC* testProc;
extern DEBUG_PROC* debugProc;

GameSceneManager *sceneManager;
extern void initGame();//Entry point to setup our game
//PROCS TO RUN

void GameProcUpdate()//updates our game scene
{
     //ecs.UpdateSystem();
    gameManager->runGameLoop();
    renderSystem->mainCamera.UpdateCamera();
    renderSystem->RenderScene();
    if (GetGamepadButtonPressed() == GAMEPAD_BUTTON_MIDDLE_LEFT)
        gameManager->destroyGameManager();
}

IN_GAME_PROC gameProcDemo = IN_GAME_PROC(initGame,GameProcUpdate);


void VideoProcDemo()
{
    if(!Player::playbackLoop())//update to use framebuffer
    {
        ENGINE_STATES::ChangeState(ENGINE_STATES::IN_GAME);
    }
}
VIDEO_PROC videoProcDemo = VIDEO_PROC(nullptr,VideoProcDemo);

void DemoLoadingProc()
{
    if(loadingOverlay == nullptr)
    {
        loadingOverlay = new LoadingOverlay();
    }
    BeginDrawing();
    ClearBackground(RAYWHITE);
    loadingOverlay->Draw();
    EndDrawing();
}
LOADING_PROC loadingProcDemo = LOADING_PROC(nullptr,DemoLoadingProc);

void BOOT()//Move to render system
{
    if(bootOverlay == nullptr)
    {   
        bootOverlay = new BootOverlay();
    }
    BeginDrawing();
    ClearBackground(WHITE);
    bootOverlay->Draw();
    EndDrawing();
    
    timer += GetFrameTime();
    if (timer > 5)
    {
        timer = 0;
      
        RunProc(&gameProcDemo);
    }

}
BOOT_PROC bootProcDemo = BOOT_PROC(nullptr,BOOT);

void Wait()//move to render system
{
    if(loadingOverlay == nullptr)
    {
        loadingOverlay = new LoadingOverlay();
    }
    BeginDrawing();
    ClearBackground(RAYWHITE);
                loadingOverlay->Draw();
    EndDrawing();
    timer += GetFrameTime();
    if (timer > 2)
    {
        timer = 0;
        ENGINE_STATES::ChangeState(ENGINE_STATES::IN_GAME);
    }
}

IDLE_PROC idleProcDemo = IDLE_PROC(nullptr,Wait);

/**
 * @brief Initializes the engine.
 * @details This function is responsible for initializing the engine. It includes initializing the romFS, initializing the debug log, initializing the game objects, initializing the physics world, initializing the ECS, and setting the initial state of the engine.
 */
void initSystem()
{   
    romfsInit();
    debugLogInit();
    debugLog("romFS Init");
    debugLog("System Starting...");
    SDL_Init(SDL_INIT_AUDIO);
    Mix_Init(MIX_INIT_MP3);
    debugLog("SDL Mixer Init");
    physicsWorld = new PhysicsWorld();
    physicsWorld->InitBasic();
    physicsWorld->InitBullet();
    ecs.InitSystem();
    debugLog("Engine Callbacks Init");
    ENGINE_STATES::ChangeState(ENGINE_STATES::BOOT);//always runs first
    videoProc = &videoProcDemo;
    loadingProc = &loadingProcDemo;
    bootProc = &bootProcDemo;
    idleProc = &idleProcDemo;

}

/**
 * @brief Main loop of the engine. Runs the current state of the engine.
 * @details This function is the main loop of the engine. It runs the current state of the engine, whether it is in-game, in a cutscene, in a menu, or in a loading screen.
 * The current state of the engine is determined by the value of ENGINE_STATES::GetState().
 * The function will run until the window is closed or the game manager's Running() function returns false.
 */
void EngineMain()
{
    debugLog("Engine Starting...");
    gameManager = new GameManager(true);
    gameManager->Init();
    while (!WindowShouldClose() && gameManager->Running())
    {
        if(ENGINE_STATES::GetState() == ENGINE_STATES::IN_GAME)
        {
            inGameProc->RUN();
        }
        else if (ENGINE_STATES::GetState() == ENGINE_STATES::BOOT)
        {
            bootProc->RUN();
        }
        else if(ENGINE_STATES::GetState() == ENGINE_STATES::VIDEO)
        {
            videoProc->RUN();
        }
        else if(ENGINE_STATES::GetState() == ENGINE_STATES::MENU)
        {
            menuProc->RUN();
        }
        else if (ENGINE_STATES::GetState() == ENGINE_STATES::IDLE)
        {
            idleProc->RUN();
        }
        else if(ENGINE_STATES::GetState() == ENGINE_STATES::LOADING)
        {
            loadingProc->RUN();
        }
         else if(ENGINE_STATES::GetState() == ENGINE_STATES::TEST)
        {
            testProc->RUN();
        }
    }

}
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    initSystem();
    EngineMain();
    //--------------------------------------------------------------------------------------
    physicsWorld->ShutdownPhysicsWorld();
    debugLog("Game Stopped....");
    debugLogCleanup();
    romfsExit();
    CloseWindow();
    debugLog("Exiting....");
    appletExit();
    return EXIT_SUCCESS;

}



