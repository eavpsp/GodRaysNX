#ifndef ROBOTANIMATIONCONTROLLER_H
#define ROBOTANIMATIONCONTROLLER_H
#include <AnimationController.h>

enum RobotAnimDataIndex
{
    IDLE,
    WALK,
    JUMP,
};
struct RobotAnimData : public AnimationControllerData//store data like triggers and floats
{
    RobotAnimData(){};
    ~RobotAnimData(){};
};

struct RobotAnimationController : public AnimationController//control animation how we want
{

    void DoIdle();
    void DoRun();
    RobotAnimationController();
    ~RobotAnimationController();
    void OnUpdate() override;
};


#endif // ROBOTANIMATIONCONTROLLER_H
