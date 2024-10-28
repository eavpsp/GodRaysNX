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
-------------------------
*WIP
___________________________
**Current
~Physics 2D Component - WIP
~Update Editor Components For Scene Loading- WIP
    `Loader - 
    `Editor -
    -set  up box and handle collisions
PBR as Shader Option -


---------------------------
*NOT STARTED
__________________________
----------------------------
*FUTURE IMPLEMENTATIONS/OPTIMIZATIONS
_____________________________
Multithreading -
    Thread Pool-
    Task Scheduler-
    Physics Thread -
    Audio/Video Processing Thread -
    Asset Loading Thread -
        Game Scene Loader -
Animation Events -
IK -
Procedural Animation -
File Archive System -
ARM SIMD Instruction Set Implementation - 
Add Smart Pointers -
Networking - 
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
EngineCallBacks *engineCallBacks;
extern RenderSystem *renderSystem;
PhysicsWorld *physicsWorld;
//CALL BACK VARIABLES
std::vector<GameObject *> *GameObjects;
std::vector<PhysicsComponent *> *PhysicsObjects;
//LOCAL OVERLAYS
BootOverlay *bootOverlay;
LoadingOverlay *loadingOverlay;

//LOCAL VARS
static float timer = 0;

//Get all system procs
extern CUTSCENE_PROC* cutsceneProc;
extern VIDEO_PROC* videoProc;
extern MENU_PROC* menuProc;
extern IN_GAME_PROC* inGameProc;
extern IDLE_PROC* idleProc;
extern LOADING_PROC* loadingProc;
extern BOOT_PROC* bootProc;
extern TEST_PROC* testProc;
extern DEBUG_PROC* debugProc;


//PROCS TO RUN
void TestPhysicsBullet()
{
    Texture2D texture = LoadTexture("romfs:/textures/nosignal.png");
    GameObject *plane = GameObject::InstantiateGameObject<GameObject>(Vector3{0,-5,0}, Quaternion{0,0,0,0},20.0f);
    GR_Mesh* planeData = new GR_Mesh(LoadModel("romfs:/models/prim/plane.obj"));
    GR_MeshComponent *planeMesh = new GR_MeshComponent(planeData);
    planeMesh->SetShader(renderSystem->defaultShader);
    planeMesh->SetTint(RED);
    plane->AddComponent(planeMesh);
    BulletPhysicsComponent *planeBody = new BulletPhysicsComponent(plane->position, plane->rotation, 0.0f, new btBoxShape(btVector3(20.0f,0.5f,20.0f)));      
    plane->AddComponent(planeBody);
    
    GameObject *slope = GameObject::InstantiateGameObject<GameObject>(Vector3{0,0,0}, Quaternion{0,180,0,0}, 2.0f);
    GR_Mesh* slopeData = new GR_Mesh(LoadModel("romfs:/models/prim/cube.obj"));
    GR_MeshComponent *slopeMesh = new GR_MeshComponent(slopeData);
    slopeMesh->SetShader(renderSystem->defaultShader);
    slopeMesh->SetTint(GREEN);
    slope->AddComponent(slopeMesh);
    BulletPhysicsComponent *slopeBody = new BulletPhysicsComponent(slope->position, slope->rotation, 0.0f, new btBoxShape(btVector3(10.0f,2.0f,10.0f)));      
    slope->AddComponent(slopeBody);
   
    GameObject *meshObject = GameObject::InstantiateGameObject<GameObject>(Vector3{0,10,0}, Quaternion{0,0,0,0}, 2.0f);
    GR_Mesh* meshData = new GR_Mesh(LoadModel("romfs:/models/prim/sphere.obj"));
    GR_MeshComponent *mesh = new GR_MeshComponent(meshData);
    mesh->SetShader(renderSystem->defaultShader);
    mesh->SetTexture(texture);
    meshObject->AddComponent(mesh);
    BulletPhysicsComponent *meshBody = new BulletPhysicsComponent(meshObject->position, meshObject->rotation, 1.0f, new btSphereShape(1.2f));      
    meshObject->AddComponent(meshBody);

}   
void GameProcDemo()
{
     //ecs.UpdateSystem();
    gameManager->runGameLoop();
    renderSystem->mainCamera.UpdateCamera();
    renderSystem->RenderScene();
    //Run Update Callbacks//
    engineCallBacks->PhysicsUpdate();
    engineCallBacks->RunUpdateCallbacks();
    // Get and process input
    if (GetGamepadButtonPressed() == GAMEPAD_BUTTON_MIDDLE_LEFT)
        gameManager->destroyGameManager();
}

IN_GAME_PROC gameProcDemo = IN_GAME_PROC(TestPhysicsBullet,GameProcDemo);


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
        ENGINE_STATES::ChangeState(ENGINE_STATES::IDLE);
    }
}

IDLE_PROC idleProcDemo = IDLE_PROC(nullptr,Wait);





void initSystem()
{   
    romfsInit();
    debugLogInit();
    debugLog("romFS Init");
    debugLog("System Starting...");
    SDL_Init(SDL_INIT_AUDIO);
    Mix_Init(MIX_INIT_MP3);
    debugLog("SDL Mixer Init");
    GameObjects = new std::vector<GameObject *>();
    engineCallBacks = new EngineCallBacks();
    physicsWorld = new PhysicsWorld();
    physicsWorld->InitBasic();
    physicsWorld->InitBullet();
    PhysicsObjects = new std::vector<PhysicsComponent *>();
    ecs.InitSystem();
    debugLog("Engine Callbacks Init");
    ENGINE_STATES::ChangeState(ENGINE_STATES::BOOT);//always runs first
    videoProc = &videoProcDemo;
    loadingProc = &loadingProcDemo;
    bootProc = &bootProcDemo;
    idleProc = &idleProcDemo;

}

void EngineMain()
{
    debugLog("Engine Starting...");
    //init GM
    gameManager = &GameManager::getGameManager();
    while (!WindowShouldClose() && gameManager->Running())      // Detect window close button
    //move states to game manager
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
        else if(ENGINE_STATES::GetState() == ENGINE_STATES::CUTSCENE)
        {
            //Cutscene Here
            cutsceneProc->RUN();
        }
        else if(ENGINE_STATES::GetState() == ENGINE_STATES::MENU)
        {
            menuProc->RUN();
        }
        else if (ENGINE_STATES::GetState() == ENGINE_STATES::IDLE)
        {
            idleProc->RUN();
        }
        else if(ENGINE_STATES::GetState() == ENGINE_STATES::LOADING)//move to render system
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



