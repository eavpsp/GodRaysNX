#include <GamePad.h>
#include <raylib.h>

void GamePads::Init(ControllerMap *controllerMap)
{
    AddGamePad(controllerMap);
}

void GamePads::UpdateGamePad(int oldID, int newID)
{
    connectedControllers[oldID] = false;
    connectedControllers[newID] = true;
}
void GamePads::AddGamePad(ControllerMap *controllerMap) 
{
        if(connectedControllersCount + 1 < MAX_GAMEPADS)
        {
            return;
        }
        if(connectedControllersCount == 0)
        {
            connectedControllers[0] = true;
            controllerMap->controllerID = 0;
            return;
        }
        else
        {
            //add on
            controllerMap->controllerID = connectedControllersCount;
            connectedControllers[connectedControllersCount] = true;
            connectedControllersCount++;
        }
        
        
}
void GamePads::SwapControllerMap(ControllerMap *currentController, ControllerMap *newController)
{
    currentController = newController;
}

