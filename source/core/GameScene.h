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
// Holds Scene Gameobjects
// Light Data
// Camera Data

extern LoadingOverlay *loadingOverlay;
extern std::vector<GameObject *> *GameObjects;//NEED THIS FOR CALLBACKS
extern std::map<_RES::Scenes_Types, std::string> RES_Scenes;
extern std::map<int, std::string> RES_ModelAnimations;
extern std::map<int, std::string> RES_AudioFiles;
extern std::map<int, std::string> RES_Models;
static float sceneTimer = 0;

enum GameObjectType//Add custom object types for implementation
{
    BASE_OBJ, LIGHT
};
enum CameraType
{
    PERSPECTIVE, OTHOGRAPHIC  
};
//COMPONENTS 0xC??? 
struct __attribute__((packed)) ComponentDataBase
{
 
};
//If components exist add them with the data in the GRB otherwise just use default data in Gameobject loaded
struct __attribute__((packed)) AnimationComponentData : ComponentDataBase //Header for animations 0xCA00
{
    int animationID;
};
struct __attribute__((packed)) PhysicsComponentsData : ComponentDataBase//Header for physics 0xCB00
{
    float mass;
    Vector3 _size;
    bool _isTrig, _isKinematic;
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
    Vector3 scale; //float 3
    int modelID;//int
    int parentIndex;//use this to add any children to parent
    int numOfComponents;//int

};
struct __attribute__((packed)) SceneDataLoader//cast to start of .grb loads the camera for the scene
{
    u16 header;//file header 0XA0A0
    int sceneID;//int
    CameraType cameraType;//int
    Vector3 cameraPosition;//float 3
    Quaternion cameraRotation;//float4
    float gravityAmount;
    u16 SceneObjectDataLoaderHeader;//object start header 0xA4A4
    int numObjects;
   // SceneObjectData objectsInScene[];//auto sets if proper layout

};
struct GameScene
{
    _RES::Scenes_Types sceneID;
    bool isLoaded = false;  
    std::vector<SceneObjectData*> objectsInScene;
    std::map<int, std::map<int, ComponentDataBase*>> objectsAndComponentsMap;
    GameScene(_RES::Scenes_Types _sceneID) : sceneID(_sceneID)
    {
      
    }
     //.grb/
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
                for (int i = 0; i < sceneLoaded->numObjects; i++) 
                {

                    SceneObjectData *objectLoaded = (SceneObjectData *)(data + sizeof(SceneDataLoader) + i * sizeof(SceneObjectData) + addon);//works update to add header check after each obj check for component or object
                    debugLog("Object ID: %d",objectLoaded->objID);
                    switch (objectLoaded->objType)
                    {
                            case LIGHT:
                           
                            break;
                        
                            default://gameobject
                                debugLog("Game Object Found");
                                 // GameObject *obj = GameObject::InstantiateGameObject<GameObject>(objectLoaded->position, objectLoaded->rotation,objectLoaded->scale, LoadModel(RES_Models[objectLoaded->modelID].c_str()));
                                CurrentScene->objectsInScene.push_back(objectLoaded);
                                if(objectLoaded->numOfComponents > 0)
                                {
                                    for(int j = 0; j < objectLoaded->numOfComponents; j++)
                                    {
                                        u16 nextHeader = *(u16 *)(data + sizeof(SceneDataLoader) + i * sizeof(SceneObjectData) + addon + sizeof(SceneObjectData));
                                        //add components here
                                        addon += sizeof(u16);
                                        if (nextHeader == 51967)
                                        {//animation
                                            AnimationComponentData *anim = (AnimationComponentData *)(data + sizeof(SceneDataLoader) + i * sizeof(SceneObjectData) + addon + sizeof(SceneObjectData));
                                            CurrentScene->objectsAndComponentsMap[i][51967] = (anim);
                                            addon += sizeof(AnimationComponentData);
                                            debugLog("Animation Component Found");
                                        
                                        }
                                        else if(nextHeader == 52223)
                                        {
                                            
                                            PhysicsComponentsData *physics = (PhysicsComponentsData *)(data + sizeof(SceneDataLoader) + i * sizeof(SceneObjectData) + addon + sizeof(SceneObjectData));
                                            CurrentScene->objectsAndComponentsMap[i][52223] = (physics);
                                            debugLog("isKinematic: %d", physics->_isKinematic);
                                            addon += sizeof(PhysicsComponentsData);
                                            debugLog("Physics Component Found");
                                        }
                                        else if(nextHeader == 52479)
                                        {
                                            
                                            AudioComponentData *audio = (AudioComponentData *)(data + sizeof(SceneDataLoader) + i * sizeof(SceneObjectData) + addon + sizeof(SceneObjectData));
                                            CurrentScene->objectsAndComponentsMap[i][52479] = (audio);
                                            addon += sizeof(AudioComponentData);
                                            debugLog("Audio Component Found");
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
                Model model = LoadModelData(CurrentScene->objectsInScene[i]->modelID);
                obj->AddComponent(new GR_MeshComponent(new GR_Mesh(model)));
                debugLog("Number of Components: %d", CurrentScene->objectsInScene[i]->numOfComponents);
                if (CurrentScene->objectsInScene[i]->numOfComponents > 0)
                {
                    
                    for (size_t f = 0; f < CurrentScene->objectsInScene[i]->numOfComponents; f++)
                    {
                        for(auto& objComps : CurrentScene->objectsAndComponentsMap[i])
                        {
                            if(objComps.first == 51967)
                            {
                                    AnimationComponentData *anim = (AnimationComponentData *)(CurrentScene->objectsAndComponentsMap[i][51967]);//i = current obj, map key = 51967 obj is the val returned
                                    AnimationComponent *animComp = new AnimationComponent(RES_ModelAnimations[anim->animationID].c_str());
                                    obj->AddComponent(animComp);
                            }
                            else if(objComps.first == 52223)
                            {
                                    PhysicsComponentsData *physics = (PhysicsComponentsData *)(CurrentScene->objectsAndComponentsMap[i][52223]);
                                    PhysicsComponent *physicsComp = new PhysicsComponent(physics->mass, physics->_size,physics->_isTrig, physics->_isKinematic);
                                    obj->AddComponent(physicsComp);
                                    debugLog("Physics Component Added");
                            }
                            else if(objComps.first == 52479)
                            {
                                    AudioComponentData *audio = (AudioComponentData *)(CurrentScene->objectsAndComponentsMap[i][52479]);
                                    AudioComponent *audioComp = new AudioComponent(RES_AudioFiles[audio->audioID].c_str());
                                    obj->AddComponent(audioComp);
                            }
                            else
                            {
                                debugLog("Unknown Component Found %c", objComps.first);
                            }
                
                        }
                     
      
                }
                loadingOverlay->GetProgress()->SetText("%d%%", (int)((i+1.0f)/sceneLoaded->numObjects*100.0f));
                    
            }
                
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
                
               ENGINE_STATES::ChangeState(ENGINE_STATES::IDLE);
                
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
