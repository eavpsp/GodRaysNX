#include <StandardInput.h>
#include <raymath.h>

void StandardController::UpdateButtonInputs()
{
    Vector3 forward = Vector3Normalize(Vector3Subtract(camera->target, camera->position));
    //button presed -- can be extended to all button positions
     switch (GetGamepadButtonPressed())
     {
        //Thumb
        case GAMEPAD_BUTTON_LEFT_THUMB:
            break;
        case GAMEPAD_BUTTON_RIGHT_THUMB:
            break;

        //Triggers
        case GAMEPAD_BUTTON_LEFT_TRIGGER_1:
            //move camera down 
            camera->position.y -= 10.0f * GetFrameTime();
            camera->target = forward;
            break;
        case GAMEPAD_BUTTON_LEFT_TRIGGER_2:
            break;
        case GAMEPAD_BUTTON_RIGHT_TRIGGER_1:
            camera->position.y += 10.0f * GetFrameTime();
            camera->target = forward;
            break;
        case GAMEPAD_BUTTON_RIGHT_TRIGGER_2:
            break;

        //Middle
        case GAMEPAD_BUTTON_MIDDLE_LEFT:
            break;
        case GAMEPAD_BUTTON_MIDDLE:
            break;
        case GAMEPAD_BUTTON_MIDDLE_RIGHT:
            break;

        //Right Face
        case GAMEPAD_BUTTON_RIGHT_FACE_UP:
            break;
        case GAMEPAD_BUTTON_RIGHT_FACE_RIGHT:
            break;
        case GAMEPAD_BUTTON_RIGHT_FACE_DOWN:
            break;
        case GAMEPAD_BUTTON_RIGHT_FACE_LEFT:
            break;

        //Left Face
        case GAMEPAD_BUTTON_LEFT_FACE_UP:
            break;
        case GAMEPAD_BUTTON_LEFT_FACE_RIGHT:
            break;
        case GAMEPAD_BUTTON_LEFT_FACE_DOWN:
            break;
        case GAMEPAD_BUTTON_LEFT_FACE_LEFT:
            break;
        default:
            break;
     }
     

}

void StandardController::UpdateAxisInputs()
{
    float Leftx = GetGamepadAxisMovement(controllerID, GAMEPAD_AXIS_LEFT_X);
    float Lefty = GetGamepadAxisMovement(controllerID, GAMEPAD_AXIS_LEFT_Y);
    float Rightx = GetGamepadAxisMovement(controllerID, GAMEPAD_AXIS_RIGHT_X);
    float Righty = GetGamepadAxisMovement(controllerID, GAMEPAD_AXIS_RIGHT_Y);
   
   if(Leftx != 0 || Lefty != 0)
   {
        // move camera forward in the camera's view direction
        Vector3 forward = Vector3Normalize(Vector3Subtract(camera->target, camera->position));
        Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, camera->up));
        
        Vector3 direction = Vector3Add(Vector3Scale(forward, Lefty) , Vector3Scale(right, Leftx));
        camera->position = Vector3Add(camera->position, Vector3Scale(direction, 10.0f * GetFrameTime()));
        camera->target = Vector3Add(camera->position, forward);
   }
   if(Rightx != 0 || Righty != 0)
   {
        //look around
        if(Righty != 0)
        {
            //look up and down
            // Rotate camera up and down
            Vector3 forward = Vector3Normalize(Vector3Subtract(camera->target, camera->position));
            Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, camera->up));
            Vector3 up = Vector3Normalize(Vector3CrossProduct(right, forward));

            // Calculate pitch rotation matrix
            Matrix pitch = MatrixRotate(right, Righty * 2.0f * GetFrameTime());

            // Apply rotation to forward vector
            forward = Vector3Transform(forward, pitch);

            // Update camera target
            camera->target = Vector3Add(camera->position, forward);
        }
        else if(Rightx != 0)
        {
              camera->target = Vector3Add(camera->target, Vector3Scale(Vector3Normalize(Vector3CrossProduct(camera->up, Vector3Subtract(camera->target, camera->position))), Rightx * 10.0f * GetFrameTime()));

        }
   }

}