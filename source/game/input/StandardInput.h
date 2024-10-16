#ifndef STANDARDINPUT_H
#define STANDARDINPUT_H
#include<GamePad.h>
#include<raylib.h>
struct StandardController : ControllerMap
{
    void UpdateButtonInputs() override;
    void UpdateAxisInputs() override;
    Camera *camera;
    ~StandardController() { }
    StandardController(Camera* playerToControl){ camera = playerToControl; }
    StandardController() { }
};
#endif // STANDARDINPUT_H
