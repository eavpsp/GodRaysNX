#ifndef GAMESCENE_H
#define GAMESCENE_H
#include <GameManager.h>
#include <GameObject.h>
#include "../debug/debug.h"
#include <GameOverlays.h>
#include <AudioComponent.h>
#include <PhysicsComponent.h>
#include <AnimationComponent.h>
#include <switch.h>
#include <GR_MeshComponent.h>
#include <RenderSystem.h>
#include <PhysicsWorld.h>
//TODO:
//Loading Overlays /Overlay exporter
//Add loading for new components
//add functionality for cameras
//prepare the resource table
extern PhysicsWorld *physicsWorld;
extern RenderSystem *renderSystem;
extern LoadingOverlay *loadingOverlay;
extern std::vector<GameObject *> *GameObjects;//NEED THIS FOR CALLBACKS
extern std::map<_RES::Scenes_Types, std::string> RES_Scenes;
extern std::map<int, std::string> RES_ModelAnimations;
extern std::map<int, std::string> RES_AudioFiles;
extern std::map<int, std::string> RES_Models;
extern std::map<int, std::string> RES_Textures;
static float sceneTimer = 0;
enum ComponentType : char16_t
{ 
    ANIMATION = 0x6261, BULLET = 0x6270, AUDIO = 0x6361, PHYSICS = 0x6210, TEXTURE = 0x6374, MESH = 0x636D, PHYSICS2D = 0x6D70, PARTICLESS = 0x6C70
};
enum GameObjectType//Add custom object types for implementation
{
    BASEOBJ, PLIGHT, DLIGHT, SLIGHT,
};

//COMPONENTS 0xC??? 
struct __attribute__((packed)) ComponentDataBase
{
 
};
struct __attribute__((packed)) BulletComponent : ComponentDataBase//Header for bullet 0xC000
{
    //mass , shape, size, 
    float mass, radius;
    int shapeType;
    Vector3 _size;
};
struct __attribute__((packed)) Physics2DComponentsData : ComponentDataBase//Header for bullet 0xC000
{
    //mass , shape, size, 
    float mass, radius;
    int shapeType;
    bool _isTrig, _isKinematic, _useGravity;
    Vector2 _size;
};
//If components exist add them with the data in the GRB otherwise just use default data in Gameobject loaded
struct __attribute__((packed)) TextureComponentData : ComponentDataBase //Header for animations 0xCA00
{
    int textureID;
};
struct __attribute__((packed)) MeshComponentData : ComponentDataBase //Header for animations 0xCA00
{
    int meshID;
};
struct __attribute__((packed)) AnimationComponentData : ComponentDataBase //Header for animations 0xCA00
{
    int animationID;
};
struct __attribute__((packed)) PhysicsComponentsData : ComponentDataBase//Header for physics 0xCB00
{
    float mass, radius;
    Vector3 _size;
    bool _isTrig, _isKinematic, _useGravity;
    int shapeType;
};
struct __attribute__((packed)) AudioComponentData : ComponentDataBase//Header for audio 0xCC00
{
    int audioID;
};
//OBJECTS
struct __attribute__((packed)) SceneObjectData//add data for children and parent
{
    u16 header;//object start header 0xAAAA end
    GameObjectType objType; //int custom class if needed
    int objID;//int unique id
    Vector3 position; //float 3
    Quaternion rotation; //float 4
    float scale; //float 1
    bool isStatic;
    int parentIndex;//use this to add any children to parent
    int numOfComponents;//int//add int for num of children

};
struct __attribute__((packed)) SceneDataLoader//cast to start of .grb loads the camera for the scene//stage size 
{
    u16 header;//file header 0XA0A0
    int sceneID;//int
    Vector3 cameraPosition, target, stageMax, stageMin;//float 3 bounds max /min
    float gravityAmount;
    u16 SceneObjectDataLoaderHeader;
    int numObjects;
  

};
struct GameScene
{
    _RES::Scenes_Types sceneID;
    bool isLoaded = false;  
    std::vector<SceneObjectData*> objectsInScene;
    std::map<int, std::map<char16_t, ComponentDataBase*>> objectsAndComponentsMap;
    StaticQuadTreeContainer<GameObject> *quadTreeContainer;//add dynamic as well?
    BoundingBox stageBounds;
    GameScene(_RES::Scenes_Types _sceneID) : sceneID(_sceneID)
    {
        
    }
     //.grb//
};
class GameSceneManager
{
    private:
    Model LoadModelData(int id)
    {
        if(id == -1)
        {
            return Model();
        }
        return LoadModel(RES_Models[id].c_str());
    }
    
/**
 * Loads a scene from a file with the given path. The file is expected to
 * contain a SceneDataLoader struct at the start of the file, which contains
 * information about the camera and the objects in the scene.
 *
 * The function reads the file, checks the header, and then iterates over the
 * objects in the scene and creates a GameObject for each one.
 *
 * @param path The path to the file containing the scene data.
 *
 * @return true if the scene was loaded successfully, false otherwise.
 */
        bool LoadData(const char *path)//Loads GRB Scene Data
        {
            u16 addon;
            FILE *fp = fopen(path, "rb");
            if(fp == NULL)
            {
                printf("File not found: %s", path);
                return false;
            }

            fseek(fp, 0, SEEK_END);
            int size = ftell(fp);
            fseek(fp, 0, SEEK_SET);
            char *data = new char[size];
            fread(data, 1, size, fp);

            SceneDataLoader *sceneLoaded = (SceneDataLoader *)data;
            if(sceneLoaded->header != 0xA0A0)
            {
                debugLog("Invalid .grb file");
                debugLog("Header: %c", sceneLoaded->header);
                fclose(fp);
                delete[] data;
                return false;
            }
            else
            {
                debugLog("Valid .grb file");
                debugLog("Num Objects: %d", sceneLoaded->numObjects);
                //init camera
                renderSystem->mainCamera.camToUse->position = sceneLoaded->cameraPosition;
                renderSystem->mainCamera.camToUse->target = sceneLoaded->target;
                //init physics
                physicsWorld->SetGravity(sceneLoaded->gravityAmount);
                //quadTree
                CurrentScene->stageBounds = {sceneLoaded->stageMin, sceneLoaded->stageMax};
                CurrentScene->quadTreeContainer = new StaticQuadTreeContainer<GameObject>(CurrentScene->stageBounds);
                for (int i = 0; i < sceneLoaded->numObjects; i++) 
                {

                    SceneObjectData *objectLoaded = (SceneObjectData *)(data + sizeof(SceneDataLoader) + i * sizeof(SceneObjectData) + addon);//works update to add header check after each obj check for component or object
                    debugLog("Object ID: %d",objectLoaded->objID);
                    switch (objectLoaded->objType)
                    {
                        // BASEOBJ, PLIGHT, DLIGHT, SLIGHT
                            case PLIGHT:
                            break;
                            case DLIGHT:
                            break;
                            case SLIGHT:
                            break;
                            default://gameobject
                                debugLog("Game Object Found");
                                CurrentScene->objectsInScene.push_back(objectLoaded);
                                if(objectLoaded->numOfComponents > 0)
                                {
                                    for(int j = 0; j < objectLoaded->numOfComponents; j++)
                                    {
                                        ComponentType nextHeader = *(ComponentType *)(data + sizeof(SceneDataLoader) + i * sizeof(SceneObjectData) + addon + sizeof(SceneObjectData));
                                        //add components here
                                        addon += sizeof(ComponentType);
                                        if (nextHeader == ComponentType::ANIMATION)
                                        {//animation
                                            AnimationComponentData *anim = (AnimationComponentData *)(data + sizeof(SceneDataLoader) + i * sizeof(SceneObjectData) + addon + sizeof(SceneObjectData));
                                            CurrentScene->objectsAndComponentsMap[i][ComponentType::ANIMATION] = (anim);
                                            addon += sizeof(AnimationComponentData);
                                           
                                        
                                        }
                                        else if(nextHeader == ComponentType::PHYSICS)
                                        {
                                            
                                            PhysicsComponentsData *physics = (PhysicsComponentsData *)(data + sizeof(SceneDataLoader) + i * sizeof(SceneObjectData) + addon + sizeof(SceneObjectData));
                                            CurrentScene->objectsAndComponentsMap[i][ComponentType::PHYSICS] = (physics);
                                            addon += sizeof(PhysicsComponentsData);
                                            debugLog("Physics Found");
                                           
                                        }
                                        else if(nextHeader == ComponentType::AUDIO)
                                        {
                                            
                                            AudioComponentData *audio = (AudioComponentData *)(data + sizeof(SceneDataLoader) + i * sizeof(SceneObjectData) + addon + sizeof(SceneObjectData));
                                            CurrentScene->objectsAndComponentsMap[i][ComponentType::AUDIO] = (audio);
                                            addon += sizeof(AudioComponentData);
                                     
                                        }
                                        else if(nextHeader == ComponentType::MESH)
                                        {

                                            MeshComponentData *mesh = (MeshComponentData *)(data + sizeof(SceneDataLoader) + i * sizeof(SceneObjectData) + addon + sizeof(SceneObjectData));
                                            CurrentScene->objectsAndComponentsMap[i][ComponentType::MESH] = (mesh);
                                            addon += sizeof(MeshComponentData);
                                            debugLog("Mesh Found");
                                            
                                        }
                                        else if(nextHeader == ComponentType::TEXTURE)
                                        {
                                            TextureComponentData *texture = (TextureComponentData *)(data + sizeof(SceneDataLoader) + i * sizeof(SceneObjectData) + addon + sizeof(SceneObjectData));
                                            CurrentScene->objectsAndComponentsMap[i][ComponentType::TEXTURE] = (texture);
                                            addon += sizeof(TextureComponentData);
                                        }
                                        else if(nextHeader == ComponentType::BULLET)
                                        {
                                            BulletComponent *bullet = (BulletComponent *)(data + sizeof(SceneDataLoader) + i * sizeof(SceneObjectData) + addon + sizeof(SceneObjectData));
                                            CurrentScene->objectsAndComponentsMap[i][ComponentType::BULLET] = (bullet);
                                            addon += sizeof(BulletComponent);
                                        }
                                        else if(nextHeader == ComponentType::PHYSICS2D)
                                        {
                                            Physics2DComponentsData *p2d = (Physics2DComponentsData *)(data + sizeof(SceneDataLoader) + i * sizeof(SceneObjectData) + addon + sizeof(SceneObjectData));
                                            CurrentScene->objectsAndComponentsMap[i][ComponentType::PHYSICS2D] = (p2d);
                                            addon += sizeof(Physics2DComponentsData);
                                        }
                                       
                                        
                                    }
                                }
                            break;
                    }
                    
                    
                }
               
            }

            fclose(fp);

            //gameobjects
            for (size_t i = 0; i < sceneLoaded->numObjects; i++)
            {   
                GameObject *obj = GameObject::InstantiateGameObject<GameObject>(CurrentScene->objectsInScene[i]->position, CurrentScene->objectsInScene[i]->rotation,CurrentScene->objectsInScene[i]->scale);
                if (CurrentScene->objectsInScene[i]->numOfComponents > 0)
                {
                        for(auto& objComps : CurrentScene->objectsAndComponentsMap[i])
                        {
                            debugLog("Component Found: %x", objComps.first);
                            if(objComps.first == ComponentType::ANIMATION)
                            {
                                AnimationComponentData *anim = (AnimationComponentData *)(CurrentScene->objectsAndComponentsMap[i][ComponentType::ANIMATION]);//i = current obj, map key = 51967 obj is the val returned
                                AnimationComponent *animComp = new AnimationComponent(RES_ModelAnimations[anim->animationID].c_str());
                                obj->AddComponent(animComp);
                            }
                            else if(objComps.first == ComponentType::PHYSICS)
                            {
                                PhysicsComponentsData *physics = (PhysicsComponentsData *)(CurrentScene->objectsAndComponentsMap[i][ComponentType::PHYSICS]);
                                PhysicsComponent *physicsComp = new PhysicsComponent(physics->mass, physics->_size,physics->_isTrig, physics->_isKinematic);
                                obj->AddComponent(physicsComp);
                                debugLog("Physics Component Added");
                            }
                            else if(objComps.first == ComponentType::AUDIO)
                            {
                                AudioComponentData *audio = (AudioComponentData *)(CurrentScene->objectsAndComponentsMap[i][ComponentType::AUDIO]);
                                AudioComponent *audioComp = new AudioComponent(RES_AudioFiles[audio->audioID].c_str());
                                obj->AddComponent(audioComp);
                            }
                            else if (objComps.first == ComponentType::MESH)
                            {
                        
                                MeshComponentData *mesh = (MeshComponentData *)(CurrentScene->objectsAndComponentsMap[i][ComponentType::MESH]);
                                Model model = LoadModel(RES_Models[mesh->meshID].c_str());
                                GR_MeshComponent *meshComp = new GR_MeshComponent(new GR_Mesh(model));
                                meshComp->SetShader(renderSystem->defaultShader);
                                obj->AddComponent(meshComp);
                            }
                            else if(objComps.first == ComponentType::TEXTURE)
                            {
                                TextureComponentData *textureLoaded = (TextureComponentData *)(CurrentScene->objectsAndComponentsMap[i][ComponentType::TEXTURE]);
                                Texture2DComponent *textureComp = new Texture2DComponent(RES_Textures[textureLoaded->textureID].c_str());
                                textureComp->texture2DIndex = textureLoaded->textureID;
                                obj->AddComponent(textureComp);
                            }
                            else if(objComps.first == ComponentType::BULLET)
                            {
                                BulletComponent *bullet = (BulletComponent *)(CurrentScene->objectsAndComponentsMap[i][ComponentType::BULLET]);
                                if(bullet->shapeType == 0)
                                {
                                    BulletPhysicsComponent *bulletComp = new BulletPhysicsComponent(obj->position, obj->rotation, bullet->mass, new btBoxShape(btVector3(bullet->_size.x, bullet->_size.y, bullet->_size.z)));
                                    obj->AddComponent(bulletComp);

                                }
                                else
                                {
                                    BulletPhysicsComponent *bulletComp = new BulletPhysicsComponent(obj->position, obj->rotation, bullet->mass, new btSphereShape(bullet->radius));
                                    obj->AddComponent(bulletComp);
                                }
                            }
                            else if(objComps.first == ComponentType::PHYSICS2D)
                            {
                                Physics2DComponentsData *p2d = (Physics2DComponentsData *)(CurrentScene->objectsAndComponentsMap[i][ComponentType::PHYSICS2D]);
                                if(p2d->shapeType == 0)
                                {
                                    Physics2DComponent *p2dComp = new Physics2DComponent(p2d->mass, p2d->radius, p2d->_isTrig, p2d->_isKinematic);
                                    obj->AddComponent(p2dComp);

                                }
                                else
                                {
                                    Physics2DComponent *p2dComp = new Physics2DComponent(p2d->mass, p2d->_size, p2d->_isTrig, p2d->_isKinematic);
                                    obj->AddComponent(p2dComp);
                                }
                            }

                        }
                     
                loadingOverlay->GetProgress()->SetText("%d%%", (int)((i+1.0f)/sceneLoaded->numObjects*100.0f));
                    
            }
                if(obj->isStatic)
                {
                    BoundingBox bounds = (BoundingBox){
                            (Vector3){  obj->position.x - obj->scale,
                                        obj->position.y - obj->scale,
                                        obj->position.z - obj->scale },
                            (Vector3){  obj->position.x + obj->scale,
                                        obj->position.y + obj->scale,
                                        obj->position.z + obj->scale }};
                    CurrentScene->quadTreeContainer->insert(*obj, bounds);//insert into quadtree
                }
                //create dynamic tree
                
        }
            
            
            return true;
        }

    public:
        GameScene *CurrentScene;
        GameSceneManager()
        {
            loadingOverlay = new LoadingOverlay();
        };
        ~GameSceneManager()
        {
            delete loadingOverlay;
        };
        
        /**
         * @brief Load a scene into memory
         * @details Set the current scene to the given one, unload the previous one, and update the progress bar
         * @param[in] scene The scene to be loaded
         */
        void LoadScene(GameScene *scene)
        {
            ENGINE_STATES::ChangeState(ENGINE_STATES::LOADING);
            loadingOverlay->GetProgress()->SetText("%d%%", 0);
            UnLoadScene();
            CurrentScene = scene;
            //create the game objects, add them to the scene//
            CurrentScene->isLoaded = LoadData(RES_Scenes[scene->sceneID].c_str());
            if(!CurrentScene->isLoaded)
            {
                debugLog("Failed to load scene");
                return;
            }
            else
            {
                debugLog("Loaded scene");   
               ENGINE_STATES::ChangeState(ENGINE_STATES::IN_GAME);
                
            }

        };

        /**
         * @brief Unload the current scene
         * @details Clear all game objects, and reset the current scene pointer
         */
        void UnLoadScene()
        {   
                //clearr game objects
                for (size_t i = 0; i < GameObjects->size(); i++)
                {
                    /* code */
                    delete GameObjects->at(i);
                }
                debugLog("Unloading scene");
                
        };

};

#endif // GAMESCENE_H
