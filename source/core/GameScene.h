#ifndef GAMESCENE_H
#define GAMESCENE_H
#include <GameManager.h>
#include <GameObject.h>
#include "../debug/debug.h"
#include <GameOverlays.h>
#include <switch.h>
// Holds Scene Gameobjects
// Light Data
// Camera Data

extern LoadingOverlay *loadingOverlay;
extern std::vector<GameObject *> *GameObjects;//NEED THIS FOR CALLBACKS
extern std::map<_RES::Scenes_Types, std::string> RES_Scenes;
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
//If components exist add them with the data in the GRB otherwise just use default data in Gameobject loaded
struct __attribute__((packed)) AnimationComponentData //Header for animations 0xCA00
{
    int animationID;
};
struct __attribute__((packed)) PhysicsComponentsData//Header for physics 0xCB00
{
    float mass;
    Vector3 _size;
    bool _isTrig, _isKinematic;
    int shapeType;
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

};
struct __attribute__((packed)) SceneDataLoader//cast to start of .grb loads the camera for the scene
{
    u16 header;//file header 0XA0A0
    int sceneID;//int
    CameraType cameraType;//int
    Vector3 cameraPosition;//float 3
    Quaternion cameraRotation;//float4
    u16 SceneObjectDataLoaderHeader;//object start header 0xA4A4
    int numObjects;
   // SceneObjectData objectsInScene[];//auto sets if proper layout

};
struct GameScene
{
    _RES::Scenes_Types sceneID;
    bool isLoaded = false;  
    std::vector<SceneObjectData*> objectsInScene;
    GameScene(_RES::Scenes_Types _sceneID) : sceneID(_sceneID)
    {
       objectsInScene = std::vector<SceneObjectData*>();
    }
     //.grb/
};
class GameSceneManager
{
    private:
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

                    SceneObjectData *objectLoaded = (SceneObjectData *)(data + sizeof(SceneDataLoader) + i * sizeof(SceneObjectData));//works update to add header check after each obj check for component or object
                    CurrentScene->objectsInScene.push_back(objectLoaded);//load object in same instance to add components
                    debugLog("Object ID: %d",objectLoaded->objID);

                }
               
            }
           
            fclose(fp);

            //gameobjects
            for (size_t i = 0; i < sceneLoaded->numObjects; i++)
            {   
                loadingOverlay->GetProgress()->SetText("%d%%", (int)((i+1.0f)/sceneLoaded->numObjects*100.0f));
                switch (CurrentScene->objectsInScene.at(i)->objType)
                {
                case LIGHT:
                    /* code */
                    break;
                
                default://gameobject
                        
                        GameObject *obj = GameObject::InstantiateGameObject<GameObject>(CurrentScene->objectsInScene.at(i)->position, CurrentScene->objectsInScene.at(i)->rotation,CurrentScene->objectsInScene.at(i)->scale,_RES::GetModel(_RES::Model_ID::ROBOT_ID));
                    break;
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
            //create the game objects, add them to the scene
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
                
        };

};

#endif // GAMESCENE_H
