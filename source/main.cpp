/*
/Things



                            custom z-lib compression for grb data


                            -Text only menus for now-
                            selction options
                            change text color when hilighted
                            --

                            -Demo Levels
                            Plane and Sphere

                            add more camera types to be used in game

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
View Frustrum - Done
Particle system (Dynamic Batched Software Particles)- Done
Audio Component - Done;
Level Editor - Done
-------------------------
*WIP
___________________________
**Current

Raycasting - WIP
    Raycasting System
     Include Debugging
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
//Heap 256MB
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
#include <RenderSystem.h>
#include <ParticleSystem.h>
extern std::map<int, std::string> RES_ModelAnimations;
LoadingOverlay *loadingOverlay;
GameSceneManager *sceneManager;
EngineCallBacks *engineCallBacks;
std::vector<GameObject *> *GameObjects;
std::vector<PhysicsComponent *> *PhysicsObjects;
GameManager *gameManager;
extern MightyCam mainCamera;
static float timer = 0;
EntityComponentSystem ecs;
extern RenderSystemECS renderSystemECS;
extern TransformSystemECS transformSystem;
//QuadTrr
StaticQuadTreeContainer<Entity> *quadTreeContainer;
BurstParticleSystem* burstParticleSystem;


/////////////TEST
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
    for (size_t i = 0; i < 1000; i++)
    {
        size_t index = EntityManager::AddEntity();
        TransformComponent transform =  TransformComponent();
        transform.position = Vector3{MW_Math::Random(0.0f, 1000.0f), 0, MW_Math::Random(0.0f, 1000.0f)};
        transform.scale = 1.0f;
        transformSystem.AddComponent(EntityManager::GetEntity(index), transform);
        ModelComponent model = ModelComponent();
        renderSystemECS.AddComponent(EntityManager::GetEntity(index), model);
        BoundingBox bounds = (BoundingBox){(Vector3){ transform.position.x - transform.scale/2,
                                     transform.position.y - transform.scale/2,
                                     transform.position.z - transform.scale/2 },
                          (Vector3){ transform.position.x + transform.scale/2,
                                     transform.position.y + transform.scale/2,
                                     transform.position.z + transform.scale/2 }};
        quadTreeContainer->insert(EntityManager::GetEntity(index), bounds);
    }
    debugLog("DOTS Init");
    debugLog("Number of QuadTreeNodes: %d", quadTreeContainer->size());
}

void TestParticles()
{
    burstParticleSystem = new BurstParticleSystem(10, Vector3{0,0,0});

}
void TestLoadSceneFromEditor()
{
    sceneManager = new GameSceneManager();
    GameScene *scene = new GameScene(_RES::Scenes_Types::TEST_SCENE);
    sceneManager->LoadScene(scene);//convert to load scene
}




////////////END TEST--------------------










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
    ecs.InitSystem();
    debugLog("Engine Callbacks Init");
    ENGINE_STATES::ChangeState(ENGINE_STATES::BOOT);
    BoundingBox stageBounds = {Vector3{-1000,-1000,-1000}, Vector3{1000,1000,1000}};
    quadTreeContainer = new StaticQuadTreeContainer<Entity>(stageBounds);
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
        ENGINE_STATES::ChangeState(ENGINE_STATES::LOADING);

        TestLoadSceneFromEditor();

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
    //Model model = _RES::GetModel(_RES::Model_ID::ROBOT_ID);     //LoadModel("romfs:/models/robot.glb");
    while (!WindowShouldClose() && gameManager->Running())      // Detect window close button or ESC key
    {
        
        if(ENGINE_STATES::GetState() == ENGINE_STATES::IN_GAME)
        {
            //ecs.UpdateSystem();
            gameManager->runGameLoop();
            mainCamera.UpdateCamera();
            BeginDrawing();//Create Render System with View Frustrum //Move to render system
            BeginMode3D(*mainCamera.camToUse);
                ClearBackground(RAYWHITE);
                gameManager->renderLoop();
                
                //Entity, Octree, Frustum
            /*   MightyBoundingBox cameraBox = mainCamera.frustum.GetFrustumBoundingBox();
               for (const auto& entity : quadTreeContainer->search(cameraBox.GetBoundingBox()))
               {
                    TransformComponent transform = transformSystem.GetComponent(*entity);
                    //make bounds
                    BoundingBox bounds = {
                                (Vector3){transform.position.x -transform.scale, 
                                            transform.position.y -transform.scale,
                                            transform.position.z -transform.scale },
                                (Vector3){transform.position.x +transform.scale,
                                            transform.position.y +transform.scale,
                                            transform.position.z +transform.scale }};
                    //check if in view
                     //check if in frustr
                        MightyBoundingBox box = MightyBoundingBox(bounds);
                        
                        cameraBox.DrawBoundingBox(BLUE);
                      if (CheckCollisionBoxes(cameraBox.GetBoundingBox(), box.GetBoundingBox()) && !mainCamera.IsObjectBehindCamera(box))
                     {
                           renderSystemECS.DrawEntities(model, *entity);
                        box.DrawBoundingBox(GREEN);

                      }
                      else{
                        box.DrawBoundingBox(RED);
                      }
                            
                     
               }
               
            */
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