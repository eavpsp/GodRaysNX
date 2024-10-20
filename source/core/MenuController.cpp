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

void MenuController::OpenMenu()
{
    isActive = true;
    //add to vector of menus
}

void MenuController::CloseMenu()
{
    //remove from vector of menus
}



void Menu::DrawItems()
{
    for (size_t i = 0; i < menuItems.size(); i++)
    {
        menuItems[i].DrawItem();
    }
    
}
