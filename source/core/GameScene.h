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
extern Camera mainCamera;
extern LoadingOverlay *loadingOverlay;
extern std::vector<GameObject *> *GameObjects;//NEED THIS FOR CALLBACKS
enum GameObjectType
{
    BASE_OBJ, LIGHT
};
enum CameraType
{
    PERSPECTIVE, OTHOGRAPHIC  
};
struct __attribute__((packed)) SceneObjectData
{
    u16 header;//object start header 0xAAAA
    GameObjectType objType; //int
    Vector3 position; //float 3
    Quaternion rotation; //float 4
    Vector3 scale; //float 3
    int nameSize;
    char modelName[]; //char modelNameSize
    ~SceneObjectData()
    {
        delete[] modelName;
    
    }
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
    SceneObjectData objectsInScene[];//auto sets if proper layout

};
struct GameScene
{
    int sceneID;
    bool isLoaded = false;  
    std::string pathToBinary;  
    ~GameScene()
    {

    }
    GameScene(int _sceneID) : sceneID(_sceneID)
    {
        isLoaded = true;
    }
     //.grb/
};
class GameSceneManager
{
    private:

    public:
        static GameScene *CurrentScene;
        GameSceneManager()
        {
            loadingOverlay = new LoadingOverlay();
        };
        ~GameSceneManager()
        {
            delete loadingOverlay;

        };
        void LoadData(const char *path)
        {
            FILE *fp = fopen(path, "rb");
            if(fp == NULL)
            {
                printf("File not found: %s", path);
                return;
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
            }
            else
            {
                debugLog("Scene ID: %i", sceneLoaded->sceneID);
                debugLog("Camera Type: %i", sceneLoaded->cameraType);
                debugLog("Camera Position: %f %f %f", sceneLoaded->cameraPosition.x, sceneLoaded->cameraPosition.y, sceneLoaded->cameraPosition.z);
                debugLog("Camera Rotation: %f %f %f %f", sceneLoaded->cameraRotation.x, sceneLoaded->cameraRotation.y, sceneLoaded->cameraRotation.z, sceneLoaded->cameraRotation.w);
                debugLog("Object Count: %i", sceneLoaded->numObjects);
                debugLog("Object Position: %f %f %f", sceneLoaded->objectsInScene[0].position.x, sceneLoaded->objectsInScene[0].position.y, sceneLoaded->objectsInScene[0].position.z);
                debugLog("Object Rotation: %f %f %f %f", sceneLoaded->objectsInScene[0].rotation.x, sceneLoaded->objectsInScene[0].rotation.y, sceneLoaded->objectsInScene[0].rotation.z, sceneLoaded->objectsInScene[0].rotation.w);
                debugLog("Object Scale: %f %f %f", sceneLoaded->objectsInScene[0].scale.x, sceneLoaded->objectsInScene[0].scale.y, sceneLoaded->objectsInScene[0].scale.z);
                debugLog("Object Model Name: %s", sceneLoaded->objectsInScene[0].modelName);
            }

            fclose(fp);

            //gameobjects
            for (size_t i = 0; i < sceneLoaded->numObjects; i++)
            {   
                loadingOverlay->GetProgress()->SetText("%d%%", (int)((i+1.0f)/sceneLoaded->numObjects*100.0f));
                switch (sceneLoaded->objectsInScene[i].objType)
                {
                case LIGHT:
                    /* code */
                    break;
                
                default://gameobject
                        GameObject *obj = GameObject::InstantiateGameObject<GameObject>(sceneLoaded->objectsInScene[i].position, sceneLoaded->objectsInScene[i].rotation, sceneLoaded->objectsInScene[i].scale, _RES::GetModel(std::string(sceneLoaded->objectsInScene[i].modelName)));
                    break;
                }
            }
            
        }
/**
 * @brief Load a scene into memory
 * @details Set the current scene to the given one, unload the previous one, and update the progress bar
 * @param[in] scene The scene to be loaded
 */
        void LoadScene(GameScene *scene)//change state to loading use num of objects as reference for progress bar use a numbered percentage system
        {
            ENGINE_STATES::ChangeState(ENGINE_STATES::LOADING);
            loadingOverlay->GetProgress()->SetText("%d%%", 0);
            UnLoadScene();
            CurrentScene = scene;
            //create the game objects, add them to the scene


        };
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
