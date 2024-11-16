#include <SkyBoxRenderer.h>
#include <RenderSystem.h>
extern RenderSystem *renderSystem;
void SkyBoxObject::onUpdate()
{
    GameObject::onUpdate();
    SetPosition(renderSystem->mainCamera.camToUse->position);
}

SkyBoxObject::SkyBoxObject()
{
        GR_Mesh* meshData = new GR_Mesh(LoadModel("romfs:/models/sky.glb"));
        GR_MeshComponent *mesh = new GR_MeshComponent(meshData);
        Shader shader = LoadShader(RES_Shaders[_RES::ShaderFiles::UNLIT].first.c_str(), RES_Shaders[_RES::ShaderFiles::UNLIT].second.c_str());
        shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
        int ambientLoc = GetShaderLocation(shader, "ambient");
        SetShaderValue(shader, ambientLoc, (float[4]){ 0.2f, 0.2f, 0.2f, 1.0f }, SHADER_UNIFORM_VEC4);
        mesh->SetShader(shader);
        this->AddComponent(mesh);
    
    
        
};
