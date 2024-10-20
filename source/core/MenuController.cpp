#include "MenuController.h"

void MenuController::DrawMenu()
{
    menu.DrawMenuPanel();
}

void MenuController::UpdateMenu()
{
    //change selection
    for (size_t i = 0; i < menu.menuItems.size(); i++)
    {
        if (currentSelection == i)
        {
            menu.menuItems[i].selected = true;
        }
        
    }
    
}

void MenuController::OpenMenu(ControllerMap *cMap)
{
    isActive = true;
    currentControllerMap = cMap;
    controllerMap->ConnectController(currentControllerMap->controllerID);
    currentControllerMap->DisableController();

    //set to current menu object
}

void MenuController::CloseMenu()
{
    //remove from vector of menus
    currentControllerMap->ConnectController(controllerMap->controllerID);
    controllerMap->DisableController();
    isActive = false;
    //set current menu object to null
}



void Menu::DrawItems()
{
    for (size_t i = 0; i < menuItems.size(); i++)
    {
        menuItems[i].DrawItem();
    }
    
}
