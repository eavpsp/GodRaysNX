#include <stddef.h>
#ifndef GAMEPAD_H
#define GAMEPAD_H
#include <GameObject.h>
#include <map>
#include "../debug/debug.h"
#define MAX_GAMEPADS 4
//add gampads with control layouts
struct ControllerMap
{
    //get all controller inputs
    int controllerID = -1;
    bool isConnected = false;
    virtual void UpdateButtonInputs() = 0;
    virtual void UpdateAxisInputs() = 0;
    void ConnectController(int id) { isConnected = true; controllerID = id; }
    void DisableController() { isConnected = false; controllerID = -1; }
    void UpdateInputs() 
    { 
        if(!isConnected || controllerID == -1)
        {
            debugLog("Controller %d is not connected", controllerID);
            return; 
        }
        UpdateButtonInputs(); 
        UpdateAxisInputs(); 
    }
    virtual ~ControllerMap(){}
    ControllerMap() = default;
};
struct GamePads
{
    int connectedControllersCount;
    std::map<int, ControllerMap*> connectedControllers;
    void Init(ControllerMap *controllerMap);
    void UpdateGamePad(int oldID, int newID);
    void AddGamePad(ControllerMap *controllerMap);
    bool isConnected(int id){return connectedControllers[id]->isConnected;}
    void Disconnect(int id){connectedControllers[id] = nullptr;}
    void SwapControllerMap(ControllerMap *currentController, ControllerMap* newController);
    ~GamePads()
    {
        for (size_t i = 0; i < MAX_GAMEPADS; i++)
        {
            delete connectedControllers[i];
        }
        connectedControllersCount = 0;
    };
};



#endif // GAMEPAD_H
