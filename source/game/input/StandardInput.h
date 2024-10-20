#ifndef STANDARDINPUT_H
#define STANDARDINPUT_H
#include<GamePad.h>
#include<raylib.h>
#include<RenderSystem.h>
struct StandardController : ControllerMap
{
    void UpdateButtonInputs() override;
    void UpdateAxisInputs() override;
    MightyCam *mightyCam;
    float speed = 10.0f;
    float modifiedSpeed = 10.0f;
    ~StandardController() { }
    StandardController(MightyCam* playerToControl){ mightyCam = playerToControl; }
    StandardController() { }
};
#endif // STANDARDINPUT_H
