/*
/Things


                            cache data in scopes!

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
-------------------------
*WIP
___________________________
**Current
Shadow Maps!!!!! - 
    Shadow Maps Arent supported, needs baked lights 
    Or find how to support shadow maps
More Overlays(Implement System to Send overlays to RenderSystem for own render calls) 
    Screen Touch Controls
Update Editor Components For Scene Loading- WIP
        Loader - 
        Editor -
Object Pool -
    Base Class to be used universally for template type objects-
Stress Test?
    Swarm 
    Animations
    Physics
Multithreading -
    Thread Pool-
    Task Scheduler-
    Physics Thread -
    Audio/Video Processing Thread -
    Asset Loading Thread -
        Game Scene Loader
ComputeShader Pathfinding -
On Screen Debugger -
---------------------------
*NOT STARTED
__________________________
----------------------------
*FUTURE IMPLEMENTATIONS
_____________________________
Skeletal Animation - M3D
IK -
Procedural Animation -
Animation Events -
File Archive System -
ARM SIMD Instruction Set Implementation - 
Add Smart Pointers -
Networking - 
    Multiplayer -
*/
//Heap 256MB
//4GB Ram 
#define TOTAL_RAM (4ULL * 1024ULL * 1024ULL * 1024ULL)
#define MAX_LEVEL_RAM (TOTAL_RAM / 3ULL) // 4GB / 3 so 1 third of ram MAX is used for levels
#define MAX_GRB_FILE_SIZE (TOTAL_RAM / 4ULL) // ~1.2 GB //may pre load scenes in ram
#include <switch.h>
#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
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
#include <AnimationController.h>
#include <Entity.h>
#include <RenderSystem.h>
#include <RenderSystem.h>
#include <ParticleSystem.h>
#include <ResourceManager.h>
#include <ShaderInterface.h>
#include <glad/glad.h>  // glad library (OpenGL loader)
//move res stuff to scene manager//
extern std::map<int, std::string> RES_ModelAnimations;
LoadingOverlay *loadingOverlay;
GameSceneManager *sceneManager;
EngineCallBacks *engineCallBacks;
std::vector<GameObject *> *GameObjects;
std::vector<PhysicsComponent *> *PhysicsObjects;
GameManager *gameManager;
static float timer = 0;
EntityComponentSystem ecs;
extern RenderSystemECS renderSystemECS;
extern TransformSystemECS transformSystem;
StaticQuadTreeContainer<Entity> *quadTreeContainer;
BurstParticleSystem* burstParticleSystem;
extern RenderSystem *renderSystem;
PhysicsWorld *physicsWorld;
//vector of menu controllers

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
        GameObject *obj = GameObject::InstantiateGameObject<GameObject>(pos, Quaternion{0,0,0,0}, 1.0f);
        Model model = _RES::GetModel(_RES::Model_ID::ROBOT_ID);
        obj->AddComponent(new GR_MeshComponent(new GR_Mesh(model)));
        PhysicsComponent *comp = new PhysicsComponent(1.0f, 2.0f,false,  i == 0 ? false : true);
        obj->AddComponent(comp);
    }
  
    
    
}
void TestAnimations()
{
        Vector3 pos = Vector3{0, 0, 0};
        Model model = _RES::GetModel(_RES::Model_ID::ROBOT_ID);
        GameObject *obj = GameObject::InstantiateGameObject<GameObject>(pos, Quaternion{0,0,0,0}, 1.0f);
        obj->AddComponent(new GR_MeshComponent(new GR_Mesh(model)));
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

void DrawPhysicsObjects()
{
       for (size_t i = 0; i < PhysicsObjects->size(); i++)
           {
                MightyBoundingBox box = MightyBoundingBox(PhysicsObjects->at(i)->_bounds);
                box.DrawBoundingBox(box.boxColor);
           }
           
}
void TestPhysicsSAT()
{
    Shader shader = LoadShader(RES_Shaders[_RES::ShaderFiles::LIGHT].first.c_str(), RES_Shaders[_RES::ShaderFiles::LIGHT].second.c_str());
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
    int ambientLoc = GetShaderLocation(shader, "ambient");
    SetShaderValue(shader, ambientLoc, (float[4]){ 0.2f, 0.2f, 0.2f, 1.0f }, SHADER_UNIFORM_VEC4);

    //spawn slop and a mesh
    Texture2D texture = LoadTexture("romfs:/textures/nosignal.png");
    GameObject *slope = GameObject::InstantiateGameObject<GameObject>(Vector3{0,0,0}, Quaternion{0,0,0,0}, 1.0f);
    GR_Mesh* slopeData = new GR_Mesh(LoadModel("romfs:/models/prim/plane.obj"));
    GR_MeshComponent *slopeMesh = new GR_MeshComponent(slopeData);
    slopeMesh->SetShader(shader);
    slope->AddComponent(slopeMesh);
    PhysicsComponent *slopeBody = new PhysicsComponent(1.0f, Vector3{1,1,1}, false, true);
    slopeBody->_bounds.SetupColliderMesh(slopeData->model.meshes[0]);
    slope->AddComponent(slopeBody);
  
    GameObject *meshObject = GameObject::InstantiateGameObject<GameObject>(Vector3{0,10,0}, Quaternion{0,0,0,0}, 1.0f);
    GR_Mesh* meshData = new GR_Mesh(LoadModel("romfs:/models/prim/cube.obj"));
    GR_MeshComponent *mesh = new GR_MeshComponent(meshData);
    mesh->SetShader(shader);
    meshObject->AddComponent(mesh);
    PhysicsComponent *meshBody = new PhysicsComponent(1.0f, Vector3{1,1,1}, false, true);
    meshBody->_bounds.SetupColliderMesh(mesh->mesh->model.meshes[0]);
    meshObject->AddComponent(meshBody);
    

    for(int i = 0; i < 5 ; i++)
    {
        GameObject *meshObject2 = GameObject::InstantiateGameObject<GameObject>(Vector3{0,15 + i * 15,0}, Quaternion{0,0,0,0}, 1.0f);
        GR_Mesh* meshData2 = new GR_Mesh(LoadModel("romfs:/models/prim/cube.obj"));
        GR_MeshComponent *mesh2 = new GR_MeshComponent(meshData2);
        mesh2->SetShader(shader);
        meshObject2->AddComponent(mesh2);
        PhysicsComponent *meshBody2 = new PhysicsComponent(1.0f, Vector3{1,1,1}, false, false);
        meshBody2->_bounds.SetupColliderMesh(mesh->mesh->model.meshes[0]);
        meshObject2->AddComponent(meshBody2);
    }
  
    renderSystem->defaultShader = shader;
    renderSystem->SetLights();

}   


void TestPhysicsBullet()
{
    
    //spawn slop and a mesh
    GameObject *slope = GameObject::InstantiateGameObject<GameObject>(Vector3{0,0,0}, Quaternion{0,180,0,0}, 1.0f);
    GR_Mesh* slopeData = new GR_Mesh(LoadModel("romfs:/models/prim/cube.obj"));
    GR_MeshComponent *slopeMesh = new GR_MeshComponent(slopeData);
    slopeMesh->SetShader(renderSystem->defaultShader);
    slope->AddComponent(slopeMesh);
    BulletPhysicsComponent *slopeBody = new BulletPhysicsComponent(slope->position, slope->rotation, 0.0f, new btBoxShape(btVector3(1.0f,1.0f,1.0f)));      
    slope->AddComponent(slopeBody);
    Texture2D texture = LoadTexture("romfs:/textures/nosignal.png");
    GameObject *meshObject = GameObject::InstantiateGameObject<GameObject>(Vector3{0,10,0}, Quaternion{0,0,0,0}, 1.0f);
    GR_Mesh* meshData = new GR_Mesh(LoadModel("romfs:/models/prim/sphere.obj"));
    GR_MeshComponent *mesh = new GR_MeshComponent(meshData);
    mesh->SetShader(renderSystem->defaultShader);
    mesh->SetTexture(texture);
    meshObject->AddComponent(mesh);
    BulletPhysicsComponent *meshBody = new BulletPhysicsComponent(meshObject->position, meshObject->rotation, 1.0f, new btSphereShape(0.3f));      
    meshObject->AddComponent(meshBody);
    gladLoadGL();
    debugLog("Loading Shaders");
    ShaderInterface *shaderLoader = new ShaderInterface("romfs:/shaders/ShadowMap.vs", "romfs:/shaders/ppfx.fs");
////
}   


////////////END TEST--------------------










void initSystem()
{   
    //init gamepad
    romfsInit();
    debugLogInit();
    debugLog("romFS Init");
    debugLog("System Starting...");
    SDL_Init(SDL_INIT_AUDIO);
    // Load support for the MP3 format
    Mix_Init(MIX_INIT_MP3);
    debugLog("SDL Mixer Init");
    //init callbacks 
    GameObjects = new std::vector<GameObject *>();
    engineCallBacks = new EngineCallBacks();
    physicsWorld = new PhysicsWorld();
    physicsWorld->InitBasic();
    physicsWorld->InitBullet();
    PhysicsObjects = new std::vector<PhysicsComponent *>();
    ecs.InitSystem();
    debugLog("Engine Callbacks Init");
    ENGINE_STATES::ChangeState(ENGINE_STATES::BOOT);
    BoundingBox stageBounds = {Vector3{-1000,-1000,-1000}, Vector3{1000,1000,1000}};
    quadTreeContainer = new StaticQuadTreeContainer<Entity>(stageBounds);
    loadingOverlay = new LoadingOverlay();
}


void BOOT()//Move to render system
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

        TestPhysicsBullet();

    }

}
void Wait()//move to render system
{

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
        else if (ENGINE_STATES::GetState() == ENGINE_STATES::BOOT)
        {
            BOOT();
        }
        else if(ENGINE_STATES::GetState() == ENGINE_STATES::VIDEO)
        {
          
          //Video Playback Here//
            if(!Player::playbackLoop())//update to use framebuffer
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
        else if(ENGINE_STATES::GetState() == ENGINE_STATES::LOADING)//move to render system
        {
            BeginDrawing();
            ClearBackground(RAYWHITE);
                loadingOverlay->Draw();
            EndDrawing();
        }
         else if(ENGINE_STATES::GetState() == ENGINE_STATES::TEST)
        {
          
        }
        
        
    }

}
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    initSystem();
    EngineMain();
    physicsWorld->ShutdownPhysicsWorld();
    debugLog("Game Stopped....");

    debugLogCleanup();
    romfsExit();
    CloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    debugLog("Exiting....");
    appletExit();
    return EXIT_SUCCESS;

}

/*

BoundingBox GetBoundingBox(Model model) {
    BoundingBox bbox;
    bbox.min = (Vector3){FLT_MAX, FLT_MAX, FLT_MAX};
    bbox.max = (Vector3){FLT_MIN, FLT_MIN, FLT_MIN};

    // Iterate over the mesh vertices
    for (int i = 0; i < model.mesh.vertexCount; i++) {
        Vector3 vertex = model.mesh.vertices[i];
        if (vertex.x < bbox.min.x) bbox.min.x = vertex.x;
        if (vertex.y < bbox.min.y) bbox.min.y = vertex.y;
        if (vertex.z < bbox.min.z) bbox.min.z = vertex.z;
        if (vertex.x > bbox.max.x) bbox.max.x = vertex.x;
        if (vertex.y > bbox.max.y) bbox.max.y = vertex.y;
        if (vertex.z > bbox.max.z) bbox.max.z = vertex.z;
    }

    return bbox;
}*/