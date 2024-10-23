#include <RenderSystem.h>
#include <GameObject.h>
#include <ScriptCallbacks.h>
extern std::vector<GameObject *> *GameObjects;

void RenderSystem::RenderScene()
 {
    if(postProcessing)
    {
        BeginTextureMode(post_process_target);       // Enable drawing to texture
                ClearBackground(DARKBLUE);//get scene color
                BeginMode3D(*mainCamera.camToUse);


                for (size_t i = 0; i < GameObjects->size(); i++)
                {
                    /* code */
                    if(EngineCallBacks::IsValidPointer(GameObjects->at(i)))
                        GameObjects->at(i)->Draw();
                }   
                if(debugMode)
                {
                    DrawGrid(10, 1.0f);
                }

            EndMode3D();                // End 3d mode drawing, returns to orthographic 2d mode
        EndTextureMode();               // End drawing to texture (now we have a texture available for next passes)
    BeginDrawing();
            ClearBackground(DARKBLUE);//get scene color

        // Render generated texture using selected postprocessing shader
        if(IsShaderReady(postProcessingShaders[currentFX]))
        {
            debugLog("Shader Ready");
        }
        else{
            debugLog("Shader Not Ready");
        }
            BeginShaderMode(postProcessingShaders[currentFX]);
            // NOTE: Render texture must be y-flipped due to default OpenGL coordinates (left-bottom)
            DrawTextureRec(post_process_target.texture, (Rectangle){ 0, 0, (float)post_process_target.texture.width, (float)-post_process_target.texture.height }, (Vector2){ 0, 0 }, WHITE);
        EndShaderMode();
    EndDrawing();

        
    }
    else
    {
        BeginDrawing();//Create Render System with View Frustrum //Move to render system
            BeginMode3D(*mainCamera.camToUse);
                ClearBackground(DARKBLUE);//get scene color

                for (size_t i = 0; i < GameObjects->size(); i++)
                {
                    /* code */
                    if(EngineCallBacks::IsValidPointer(GameObjects->at(i)))
                        GameObjects->at(i)->Draw();
                }   
                if(debugMode)
                {
                    DrawGrid(10, 1.0f);
                }


            EndMode3D();
        EndDrawing();
    }
        
}

    