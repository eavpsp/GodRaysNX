#include <stddef.h>
#ifndef GAMEPAD_H
#define GAMEPAD_H
#include <GameObject.h>

#define MAX_GAMEPADS 4
//add gampads with control layouts
struct ControllerMap
{
    //get all controller inputs
    int controllerID;
    virtual void UpdateButtonInputs() = 0;
    virtual void UpdateAxisInputs() = 0;
    void UpdateInputs() { UpdateButtonInputs(); UpdateAxisInputs(); }
    virtual ~ControllerMap(){}
    ControllerMap() = default;
};
struct GamePads
{
    int connectedControllersCount;
    bool connectedControllers[MAX_GAMEPADS];//store if controller is connected
    void Init(ControllerMap *controllerMap);
    void UpdateGamePad(int oldID, int newID);
    void AddGamePad(ControllerMap *controllerMap);
    bool isConnected(int id){return connectedControllers[id];}
    void Disconnect(int id){connectedControllers[id] = false;}
    ~GamePads()
    {
        for (size_t i = 0; i < MAX_GAMEPADS; i++)
        {
            connectedControllers[i] = false;
        }
        connectedControllersCount = 0;
    };
};



#endif // GAMEPAD_H
