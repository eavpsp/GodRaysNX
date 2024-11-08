#include <SkyBoxRenderer.h>
#include <RenderSystem.h>
extern RenderSystem *renderSystem;
void SkyBoxObject::onUpdate()
{
    SetPosition(renderSystem->mainCamera.camToUse->position);
}

SkyBoxObject::SkyBoxObject()
{
        //add mesh and follow camera
        GR_Mesh* meshData = new GR_Mesh(LoadModel("romfs:/models/skybox.glb"));
        GR_MeshComponent *mesh = new GR_MeshComponent(meshData);
        mesh->SetShader(renderSystem->defaultShader);
        this->AddComponent(mesh);
    
        
};
