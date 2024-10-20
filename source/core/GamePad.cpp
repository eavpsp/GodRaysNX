#include <GamePad.h>
#include <raylib.h>

void GamePads::Init(ControllerMap *controllerMap)
{
    AddGamePad(controllerMap);
}

void GamePads::UpdateGamePad(int oldID, int newID)
{
    connectedControllers[oldID]->controllerID = newID;
}
void GamePads::AddGamePad(ControllerMap *controllerMap) 
{
        if(connectedControllersCount + 1 < MAX_GAMEPADS)
        {
            return;
        }
        if(connectedControllersCount == 0)
        {
            controllerMap->controllerID = 0;
            connectedControllers[controllerMap->controllerID] = controllerMap;

            return;
        }
        else
        {
            //add on
            controllerMap->controllerID = connectedControllersCount;
            connectedControllers[connectedControllersCount] = controllerMap;
            connectedControllersCount++;
        }
        
        
}
void GamePads::SwapControllerMap(ControllerMap *currentController, ControllerMap *newController)
{
    currentController = newController;
}

