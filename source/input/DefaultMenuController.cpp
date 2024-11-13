#include <DefaultMenuController.h>
#include <raymath.h>
#include<RenderSystem.h>
#include <raylib.h>
extern RenderSystem *renderSystem;
bool canMove = true;
float timer = 0;
void StandardMenuController::UpdateButtonInputs()
{
    if(!canMove)
    {
        timer += GetFrameTime();
        if(timer >= 0.2f)
        {
            canMove = true;
            timer = 0;
        }
        return;
    }
   switch (GetGamepadButtonPressed())
     {
        //Thumb
        case GAMEPAD_BUTTON_LEFT_THUMB:
            break;
        case GAMEPAD_BUTTON_RIGHT_THUMB:
            break;
        //Triggers
        case GAMEPAD_BUTTON_LEFT_TRIGGER_1:
            
            break;
        case GAMEPAD_BUTTON_LEFT_TRIGGER_2:
            break;
        case GAMEPAD_BUTTON_RIGHT_TRIGGER_1:


            break;
        case GAMEPAD_BUTTON_RIGHT_TRIGGER_2:
            break;

        //Middle START SELECT
        case GAMEPAD_BUTTON_MIDDLE_LEFT:
            break;
        case GAMEPAD_BUTTON_MIDDLE:
            break;
        case GAMEPAD_BUTTON_MIDDLE_RIGHT:
            break;

        //Right Face AB XY
        case GAMEPAD_BUTTON_RIGHT_FACE_UP:
            
            break;
        case GAMEPAD_BUTTON_RIGHT_FACE_RIGHT:
        renderSystem->activeMenus->ExecuteSelection();
        canMove = false;
            break;
        case GAMEPAD_BUTTON_RIGHT_FACE_DOWN:
        renderSystem->activeMenus->CloseMenuController();
        canMove = false;
            break;
        case GAMEPAD_BUTTON_RIGHT_FACE_LEFT:
            break;

        //Left Face DPAD
        case GAMEPAD_BUTTON_LEFT_FACE_UP:
        renderSystem->activeMenus->IncrementSelection();
        canMove = false;
            break;
        case GAMEPAD_BUTTON_LEFT_FACE_RIGHT:
            break;
        case GAMEPAD_BUTTON_LEFT_FACE_DOWN:
        renderSystem->activeMenus->DecrementSelection();
        canMove = false;
            break;
        case GAMEPAD_BUTTON_LEFT_FACE_LEFT:
            break;
        default:
            break;
     }
     
}

void StandardMenuController::UpdateAxisInputs()
{
    if(!canMove )
    {
        return;
    }
    float Leftx = GetGamepadAxisMovement(controllerID, GAMEPAD_AXIS_LEFT_X);
    float Lefty = GetGamepadAxisMovement(controllerID, GAMEPAD_AXIS_LEFT_Y);
    float Rightx = GetGamepadAxisMovement(controllerID, GAMEPAD_AXIS_RIGHT_X);
    float Righty = GetGamepadAxisMovement(controllerID, GAMEPAD_AXIS_RIGHT_Y);
   
   if(Leftx != 0 || Lefty != 0)
   {
    if(Leftx > 0)
    {
        renderSystem->activeMenus->IncrementSelection();
        canMove = false;
    }
    if (Leftx < 0)
    {
        renderSystem->activeMenus->DecrementSelection();
        canMove = false;
    }
   }
   if(Rightx != 0 || Righty != 0)
   {
        //look around
        if(Righty != 0)
        {
         
        }
         if(Rightx != 0)
        {
              
        }
   }

}