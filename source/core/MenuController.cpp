#include "MenuController.h"
#include <RenderSystem.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

extern RenderSystem *renderSystem;
void MenuController::DrawMenu()
{
    if(activeMenu == nullptr)
    {
        return;
    }
    activeMenu->DrawItems();
}

void MenuController::UpdateMenu()
{
    if(activeMenu == nullptr)
    {
        return;
    }
    if(activeMenu->currentButton != nullptr)
    {
        activeMenu->currentButton->DeselectButton();
       

    }
    activeMenu->menuItems->at(activeMenu->currentSelection).SelectButton();
    activeMenu->currentButton = &activeMenu->menuItems->at(activeMenu->currentSelection);

}

void MenuController::OpenMenuController(ControllerMap *cMap)
{
    isActive = true;
    currentControllerMap = cMap;
    controllerMap->ConnectController(currentControllerMap->controllerID);
    currentControllerMap->DisableController();
    renderSystem->activeMenus = (this);
    activeMenu = &menus->at(0);
    UpdateMenu();
    GuiLock();
    //set to current menu object
}

void MenuController::CloseMenuController()
{
    //remove from vector of menus
    currentControllerMap->ConnectController(controllerMap->controllerID);
    controllerMap->DisableController();
    isActive = false;
    activeMenu = nullptr;
     GuiUnlock();
}

void MenuController::OpenMenu(int menuIndex)
{
    if(menuIndex < 0 || menuIndex >= menus->size())
    {
        return;
    }
    activeMenu = &menus->at(menuIndex);

}

void Menu::DrawItems()
{
    for (size_t i = 0; i < menuItems->size(); i++)
    {
        menuItems->at(i).DisplayButton();
    }
    
}

void GR_Button::DisplayButton()
{
   if(isSelected)
   {
        GuiSetState(STATE_FOCUSED);
        if (GuiButton((Rectangle){  position.x, position.y, size.x, size.y },buttonText.c_str()));
        {
            action();
        }
     
      
    
   }
   else if(!isSelected)
   {
       
        GuiSetState(STATE_NORMAL);
        
        if (GuiButton((Rectangle){ position.x, position.y,size.x, size.y },buttonText.c_str()))
        {
            action();
        }
       

   }
  
}


