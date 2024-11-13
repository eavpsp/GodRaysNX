#ifndef DEFAULTMENUCONTROLLER_H
#define DEFAULTMENUCONTROLLER_H
#include<GamePad.h>

struct StandardMenuController : ControllerMap
{
    void UpdateButtonInputs() override;
    void UpdateAxisInputs() override;
    ~StandardMenuController() { }
    StandardMenuController() { }
};
#endif // DEFAULTMENUCONTROLLER_H
