#ifndef MENUCONTROLLER_H
#define MENUCONTROLLER_H

#include <string>
#include <vector>
#include "GamePad.h"
#include "GameOverlays.h"
#include <raygui.h>
#include "../debug/debug.h"
template<typename... Args>
using DelegateArgs = void (*)(Args...);
using Delegate = void (*)();
struct GR_Button
{
    //BUTTON TEXT
    //BUTTON ACTION
    std::string iconId;
    std::string buttonText;
    Delegate action;
    Vector2 size, position;
    bool isSelected = false;
    void DisplayButton();
    void SelectButton()
    {
        //resize or recolor?
        isSelected = true;
    }
    void DeselectButton()
    {
        isSelected = false;
        
    }
    GR_Button(std::string t, Delegate a, Vector2 s, Vector2 p) : buttonText(t), action(a), size(s), position(p)
    {
      
    };

};
//Gets Menu Items
//takes input from controller
//renders menu items
//show selection
//handle selection execution

//used to enable the view and draw the menu used for custom Begin Draw Methods 
struct Menu//container for menu items
{
    std::string title;
    std::vector<GR_Button> *menuItems;
    GR_Button *currentButton;
    int currentSelection = 0;
    void IncrementSelection()
    {
        currentSelection++;
    };
    void DecrementSelection()
    {
        currentSelection--;
    };
    void AddItem(GR_Button item) { menuItems->push_back(item); }
    void DrawItems();
    void CloseMenu();
    void OpenMenu();
    Menu(std::string t) : title(t) {menuItems = new std::vector<GR_Button>();};
    Menu(){menuItems = new std::vector<GR_Button>();};
    ~Menu() {};
};

//Menu 1 (SUSPENDED) -Transition from 1 to 2> Menu 2 (ACTIVE)
//Menu 2 CLOSE PUSH BACK TO PREV MENU IF ANY -> Menu 1 (ACTIVE)
//States to control menus
struct MenuController//add to vector of menus when active 
{
    private:
    ControllerMap *controllerMap;//how the menu is controlled, controls the current selection
    ControllerMap *currentControllerMap;//control map before menu was opened
    public:
    Overlay overlayToUse;//contains the menu UI elements
    Menu *activeMenu;
    std::vector<Menu> *menus;
    bool isActive;
    void DrawMenu();
    void AddMenu(Menu *menu) { menus->push_back(*menu); }
    void UpdateMenu();//update selection
    void OpenMenuController(ControllerMap *cMap);//send current controller map
    void CloseMenuController();
    void OpenMenu(int menuIndex);
    void UpdateInputs()
    {
        controllerMap->UpdateInputs();
    }
    void IncrementSelection()
    {
        if(activeMenu == nullptr)
        {
            return;
        }
        activeMenu->currentSelection = (activeMenu->currentSelection + 1) % activeMenu->menuItems->size();
        UpdateMenu();
    };
    void DecrementSelection()
    {
        if(activeMenu == nullptr)
        {
            return;
        }
        activeMenu->currentSelection = (activeMenu->currentSelection - 1 + activeMenu->menuItems->size()) % activeMenu->menuItems->size();
        //make sure loop back to zero
        UpdateMenu();

    };
    void ExecuteSelection()
    {
        if(activeMenu == nullptr)
        {
            return;
        }
        activeMenu->menuItems->at(activeMenu->currentSelection).action();
    };
    MenuController(ControllerMap *menuControllerMap) :controllerMap(menuControllerMap), isActive(false) //set the menu control map
    { 
        menus = new std::vector<Menu>();
    };
    ~MenuController() {};
};
#endif // MENUCONTROLLER_H
