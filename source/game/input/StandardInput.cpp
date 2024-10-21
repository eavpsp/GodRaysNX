#include <StandardInput.h>
#include <raymath.h>
#include <GameObject.h>
#include <PhysicsComponent.h>

GameObject *player;
bool playerActive = false;
void StandardController::UpdateButtonInputs()
{
    if(!playerActive)//physics component for camera
    {
        playerActive = true;
        player = GameObject::InstantiateGameObject<GameObject>(mightyCam->camToUse->position, Quaternion{0.0f, 0.0f, 0.0f, 0.0f}, Vector3{1.0f, 1.0f, 1.0f});
        PhysicsComponent *comp = new PhysicsComponent(1.0f, Vector3{1.0f, 1.0f, 1.0f}, false, false);
        player->AddComponent(comp);
    }
    player->position = mightyCam->camToUse->position;
    Vector3 forward = Vector3Normalize(Vector3Subtract( mightyCam->camToUse->target,  mightyCam->camToUse->position));
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
            mightyCam->SetCamPosition(Vector3Add(mightyCam->camToUse->position , Vector3{0.0f, -modifiedSpeed * GetFrameTime(), 0.0f}));
            mightyCam->camToUse->target = forward;

            break;
        case GAMEPAD_BUTTON_LEFT_TRIGGER_2:
            break;
        case GAMEPAD_BUTTON_RIGHT_TRIGGER_1:
             mightyCam->SetCamPosition(Vector3Add(mightyCam->camToUse->position , Vector3{0.0f, +modifiedSpeed* GetFrameTime(), 0.0f}));
             mightyCam->camToUse->target = forward;
            mightyCam->UpdateViewFrustum();

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
            modifiedSpeed = speed * 2.0f;//
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
            modifiedSpeed = speed;
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
        Vector3 forward = Vector3Normalize(Vector3Subtract( mightyCam->camToUse->target, mightyCam->camToUse->position));
        Vector3 right = Vector3Normalize(Vector3CrossProduct(forward,  mightyCam->camToUse->up));
        
        Vector3 direction = Vector3Add(Vector3Scale(forward, Lefty) , Vector3Scale(right, Leftx));
        mightyCam->SetCamPosition(Vector3Add( mightyCam->camToUse->position, Vector3Scale(direction, modifiedSpeed * GetFrameTime())));
        mightyCam->camToUse->target = Vector3Add( mightyCam->camToUse->position, forward);
        mightyCam->UpdateViewFrustum();
   }
   if(Rightx != 0 || Righty != 0)
   {
        //look around
        if(Righty != 0)
        {
            //look up and down
            // Rotate camera up and down
            Vector3 forward = Vector3Normalize(Vector3Subtract( mightyCam->camToUse->target,  mightyCam->camToUse->position));
            Vector3 right = Vector3Normalize(Vector3CrossProduct(forward,  mightyCam->camToUse->up));
            Vector3 up = Vector3Normalize(Vector3CrossProduct(right, forward));

            // Calculate pitch rotation matrix
            Matrix pitch = MatrixRotate(right, Righty * 2.0f * GetFrameTime());

            // Apply rotation to forward vector
            forward = Vector3Transform(forward, pitch);

            // Update camera target
             mightyCam->camToUse->target = Vector3Add( mightyCam->camToUse->position, forward);
        }
         if(Rightx != 0)
        {
               mightyCam->camToUse->target = Vector3Add( mightyCam->camToUse->target, Vector3Scale(Vector3Normalize(Vector3CrossProduct( mightyCam->camToUse->up, Vector3Subtract( mightyCam->camToUse->target,  mightyCam->camToUse->position))), Rightx * 5.0f * GetFrameTime()));

        }
   }

}