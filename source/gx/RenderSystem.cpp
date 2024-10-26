#include <RenderSystem.h>
#include <GameObject.h>
#include <ScriptCallbacks.h>
#include <GR_MeshComponent.h>
#include <rlgl.h>
#include "GameOverlays.h"
#include <raygui.h>
extern std::vector<GameObject *> *GameObjects;
//RAYGUI for ELEMENTS
void RenderSystem::DrawOverlays()
{
    for (size_t i = 0; i < overlays.size(); i++)
    {
        overlays.at(i)->Draw();
    }
    
}

void RenderSystem::DrawOverlay(Overlay *overlay)
{
    overlay->Draw();
}

void RenderSystem::AddOverlay(Overlay *overlay)
{
    overlays.push_back(overlay);
}

void RenderSystem::RemoveOverlay(Overlay *overlay)
{
    for (size_t i = 0; i < overlays.size(); i++)
    {
        if (overlays.at(i) == overlay)
        {
            overlays.erase(overlays.begin() + i);
            break;
        }
    }
}

void RenderSystem::RenderScene()
{
    if(ppfxConfig.post_processing)
    {
        Vector3 cameraPos = mainCamera.camToUse->position;
        SetShaderValue(defaultShader, defaultShader.locs[SHADER_LOC_VECTOR_VIEW], &cameraPos, SHADER_UNIFORM_VEC3);
        UpdateLightValues(defaultShader, defaultLight);
        BeginDrawing();
       //Shadow Pass
        Matrix lightView;
        Matrix lightProj;
        SetShaderValue(defaultShader, lightDirLoc, &defaultLight.position, SHADER_UNIFORM_VEC3);
        BeginTextureMode(shadowMap);
        ClearBackground(WHITE);
        BeginMode3D(lightCam);
            lightView = rlGetMatrixModelview();
            lightProj = rlGetMatrixProjection();
            for (size_t i = 0; i < GameObjects->size(); i++)
            {
            if(EngineCallBacks::IsValidPointer(GameObjects->at(i)))
               
                GameObjects->at(i)->Draw();
                
            }   
        EndMode3D();
        EndTextureMode();
        Matrix lightViewProj = MatrixMultiply(lightView, lightProj);
       //Light Pass wit Post Processing
       
        BeginTextureMode(post_process_target);
        ClearBackground(GRAY);
        SetShaderValueMatrix(defaultShader, lightVPLoc, lightViewProj);
        rlEnableShader(defaultShader.id);
        int slot = 10; // Can be anything 0 to 15, but 0 will probably be taken up
        rlActiveTextureSlot(10);
        rlEnableTexture(shadowMap.depth.id);
        rlSetUniform(shadowMapLoc, &slot, SHADER_UNIFORM_INT, 1);
        BeginMode3D(*mainCamera.camToUse);
        for (size_t i = 0; i < GameObjects->size(); i++)
        {
            if(EngineCallBacks::IsValidPointer(GameObjects->at(i)))
               
                GameObjects->at(i)->Draw();
                
        }   
        if(debugMode)
        {
            DrawGrid(10, 1.0f);
        }    
        EndMode3D();       
          EndTextureMode();        

        
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
        DrawOverlays();
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
        DrawOverlays();

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

    