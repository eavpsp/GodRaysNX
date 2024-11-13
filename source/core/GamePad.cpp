#include <GamePad.h>
#include <raylib.h>

void GamePads::Init(ControllerMap *controllerMap)
{
        controllerMap->ConnectController(connectedControllersCount);
        connectedControllers[connectedControllersCount] = controllerMap;
        connectedControllersCount++;
}

void GamePads::UpdateGamePad(int oldID, int newID)
{
    connectedControllers[oldID]->controllerID = newID;
}
void GamePads::AddGamePad(ControllerMap *controllerMap) 
{
        if(connectedControllersCount <= MAX_GAMEPADS)
        {
            return;
        }
        controllerMap->ConnectController(connectedControllersCount);
        connectedControllers[connectedControllersCount] = controllerMap;
        connectedControllersCount++;
    
        
}
void GamePads::SwapControllerMap(ControllerMap *currentController, ControllerMap *newController)
{
 
    connectedControllers[currentController->controllerID]  = newController;
    newController->ConnectController(currentController->controllerID);
    currentController->DisableController();
}

