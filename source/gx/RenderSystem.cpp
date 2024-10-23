#include <RenderSystem.h>
#include <GameObject.h>
#include <ScriptCallbacks.h>
extern std::vector<GameObject *> *GameObjects;

void RenderSystem::RenderScene()
 {
    if(postProcessing)
    {
        
        float camPos[3] = { mainCamera.camToUse->position.x, mainCamera.camToUse->position.y, mainCamera.camToUse->position.z };
        SetShaderValue(*defaultShader, defaultShader->locs[SHADER_LOC_VECTOR_VIEW], camPos, SHADER_UNIFORM_VEC3);
        UpdateLightValues(*defaultShader, defaultLight);
        BeginTextureMode(post_process_target);       // Enable drawing to texture
            ClearBackground(BLACK);//get scene color
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
            ClearBackground(BLACK);//get scene color

        // Render generated texture using selected postprocessing shader
        if(!IsShaderReady(postProcessingShaders[currentFX]))
        {
            debugLog("Shader Not Ready");
            postProcessing = false;
            return;
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

RenderSystem &RenderSystem::getRenderSystem()
{
    static RenderSystem *rs = nullptr;
    if(rs == nullptr)
    {

        rs = new RenderSystem();
        
    }
    return *rs;
}

    