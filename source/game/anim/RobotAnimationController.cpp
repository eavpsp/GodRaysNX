#include <RobotAnimationController.h>

void RobotAnimationController::DoIdle()//Should be able to call this by calling get component on animation component and getting the animation controller property
{
    SetBool(IDLE, true);
  
    
}
void RobotAnimationController::DoRun()
{
    SetTrigger(JUMP);
}

void RobotAnimationController::OnUpdate()//ran on gameobject update components call if exists
{
    if(GetBool(IDLE))
    {
        animationComponent->Play(IDLE);
    }
    else if (GetTrigger(JUMP))
    {
       animationComponent->Play(JUMP);
       ResetTrigger(JUMP);
    }
    
}
