#ifndef MENUCONTROLLER_H
#define MENUCONTROLLER_H

#include <string>
#include <vector>
#include "GamePad.h"
#include "GameOverlays.h"
//Gets Menu Items
//takes input from controller
//renders menu items
//show selection
//handle selection execution
template<typename... Args>
using Delegate = void (*)(Args...);
struct MenuItem//template class
{
    std::string text;
    Delegate<void()> delegate;
    bool selected = false;
    virtual void DrawItem(){}; 
    MenuItem(std::string t, Delegate<void()> d) : text(t), delegate(d) {};
    virtual ~MenuItem() {};
};


struct Menu//container for menu items
{
    std::string title;
    std::vector<MenuItem> menuItems;
    void AddItem(MenuItem item) { menuItems.push_back(item); }
    void DrawItems();
    virtual void DrawMenuPanel();
    virtual ~Menu() {};
    Menu(std::string t) : title(t) {};
    Menu(){};
};


struct MenuController//add to vector of menus when active 
{
    private:
    ControllerMap *controllerMap;//how the menu is controlled
    ControllerMap *currentControllerMap;//control map before menu was opened
    public:
    Menu menu;
    bool isActive;
    int currentSelection = 0;
    void DrawMenu();
    void UpdateMenu();//update selection
    void OpenMenu(ControllerMap *cMap);//send current controller map
    void CloseMenu();
    virtual void ExecuteSelection() = 0;
    MenuController(ControllerMap *menuControllerMap) :controllerMap(menuControllerMap) //set the menu control map
    { };
    virtual ~MenuController() {};
};
#endif // MENUCONTROLLER_H
