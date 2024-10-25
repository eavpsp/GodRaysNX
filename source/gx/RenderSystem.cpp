#include <RenderSystem.h>
#include <GameObject.h>
#include <ScriptCallbacks.h>
#include <GR_MeshComponent.h>
extern std::vector<GameObject *> *GameObjects;

void RenderSystem::RenderScene()
 {
    if(ppfxConfig.post_processing)
    {
        float camPos[3] = { mainCamera.camToUse->position.x, mainCamera.camToUse->position.y, mainCamera.camToUse->position.z };
        SetShaderValue(defaultShader, defaultShader.locs[SHADER_LOC_VECTOR_VIEW], camPos, SHADER_UNIFORM_VEC3);
        UpdateLightValues(defaultShader, defaultLight);
  
        BeginTextureMode(post_process_target);
        ClearBackground(GRAY);
        BeginMode3D(*mainCamera.camToUse);
        for (size_t i = 0; i < GameObjects->size(); i++)
        {
            if(EngineCallBacks::IsValidPointer(GameObjects->at(i)))
                GameObjects->at(i)->GetComponent<GR_MeshComponent>()->mesh->model.materials->maps[3].texture = shadowMap.texture; 
                GameObjects->at(i)->Draw();
                
        }   
        if(debugMode)
        {
            DrawGrid(10, 1.0f);
        }    
        EndMode3D();       
          EndTextureMode();        

        BeginDrawing();
        ClearBackground(GRAY);
        if(!IsShaderReady(postProcessingShaders))
        {
            debugLog("Shader Not Ready");
            ppfxConfig.post_processing = false;
            return;
        }

        BeginShaderMode(postProcessingShaders);
        DrawTextureRec(post_process_target.texture, (Rectangle){ 0, 0, (float)post_process_target.texture.width, (float)-post_process_target.texture.height }, (Vector2){ 0, 0 }, WHITE);
        EndShaderMode();
     
        for (size_t i = 0; i < renderProcs.size(); i++)
        {
            renderProcs.at(i).draw();
        }
        EndDrawing();
    }
    else
    {
        BeginDrawing();//Create Render System with View Frustrum //Move to render system
        BeginMode3D(*mainCamera.camToUse);
        ClearBackground(GRAY);//get scene color

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
        for (size_t i = 0; i < renderProcs.size(); i++)
        {
            renderProcs.at(i).draw();
        }
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

    