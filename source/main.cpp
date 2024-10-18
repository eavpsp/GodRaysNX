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
Physics system - Done
Input System - Done
Animation System - Done
ESC System for Static Objects - Done
-------------------------
*WIP
___________________________
**Current
View Frustrum -
    Camera System
    Render System
Particle system (Dynamic Batched Software Particles)- WIP
---------------------------
*NOT STARTED
__________________________
More Overlays(Implement System to Send overlays to GM for own render calls)  
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
#include <AnimationController.h>
#include <Entity.h>
#include <RenderSystem.h>
extern std::map<int, std::string> RES_ModelAnimations;
LoadingOverlay *loadingOverlay;
GameSceneManager *sceneManager;
EngineCallBacks *engineCallBacks;
std::vector<GameObject *> *GameObjects;
std::vector<PhysicsComponent *> *PhysicsObjects;
GameManager *gameManager;
extern MightyCam mainCamera;
static float timer = 0;


void initSystem()
{   
    //init gamepad
    romfsInit();
    debugLogInit();
    debugLog("romFS Init");
    debugLog("System Starting...");
    //init callbacks 
    GameObjects = new std::vector<GameObject *>();
    engineCallBacks = new EngineCallBacks();
    PhysicsWorld::Init();
    PhysicsObjects = new std::vector<PhysicsComponent *>();
    EntityComponentSystem::InitSystem();
    debugLog("Engine Callbacks Init");
    ENGINE_STATES::ChangeState(ENGINE_STATES::BOOT);
}

void TestVideo()
{

   Player::playbackInit("/video/test.mp4");
}   

void TestLoadScene()
{
   
    sceneManager = new GameSceneManager();
    GameScene *scene = new GameScene(_RES::Scenes_Types::ROBOT_SCENE);
    sceneManager->LoadScene(scene);//convert to load scene
    
}
void TestPhysics()
{

    for (size_t i = 0; i < 2; i++)
    {
        Vector3 pos = Vector3{0, 10 * i, 0};
        GameObject *obj = GameObject::InstantiateGameObject<GameObject>(pos, Quaternion{0,0,0,0}, Vector3{1,1,1}, _RES::GetModel(_RES::Model_ID::ROBOT_ID));
        PhysicsComponent *comp = new PhysicsComponent(1.0f, 2.0f,false,  i == 0 ? false : true);
        obj->AddComponent(comp);
    }
  
    
    
}
void TestAnimations()
{
        Vector3 pos = Vector3{0, 0, 0};
        GameObject *obj = GameObject::InstantiateGameObject<GameObject>(pos, Quaternion{0,0,0,0}, Vector3{1,1,1}, _RES::GetModel(_RES::Model_ID::ROBOT_ID));
        AnimationComponent *animComp = new AnimationComponent(RES_ModelAnimations[_RES::Model_ID::ROBOT_ID].c_str());
        AnimationControllerData *data = new AnimationControllerData();
        AnimationController *anim = new AnimationController(animComp, data);
        obj->AddComponent(animComp);
}


void TestDOTS()
{
   
    //load 100 objs in dif locations
    for (size_t i = 0; i < 100; i++)
    {
        size_t index = EntityManager::AddEntity();
        TransformComponent transform =  TransformComponent();
        transform.position = Vector3{MW_Math::Random(0.0f, 100.0f), 0, MW_Math::Random(0.0f, 100.0f)};
        transform.scale = 1.0f;
        TransformSystemESC::AddComponent(EntityManager::GetEntity(index), transform);
        ModelComponent model = ModelComponent();
        model.modelIndex = 0;
        model.modelScale = 1.0f;
        RenderSystemESC::AddComponent(EntityManager::GetEntity(index), model);

    }
    debugLog("DOTS Init");
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

        TestDOTS();

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
    Model model = LoadModel("romfs:/models/robot.glb");
    while (!WindowShouldClose() && gameManager->Running())    // Detect window close button or ESC key
    {
        
        if(ENGINE_STATES::GetState() == ENGINE_STATES::IN_GAME)
        {
            EntityComponentSystem::UpdateSystem();
            gameManager->runGameLoop();
            BeginDrawing();//Create Render System with View Frustrum //Move to render system
            BeginMode3D(*mainCamera.camToUse);
                ClearBackground(RAYWHITE);
                gameManager->renderLoop();
                RenderSystemESC::Draw(model);
               // DrawModel(model, Vector3{0,0,0}, 1.0f, WHITE);
            EndMode3D();
            EndDrawing();
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
            ClearBackground(RAYWHITE);
                loadingOverlay->Draw();
            EndDrawing();
        }
         else if(ENGINE_STATES::GetState() == ENGINE_STATES::TEST)
        {
            BeginDrawing();
             //   
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