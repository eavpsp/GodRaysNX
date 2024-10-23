#ifndef GX_GR_MESH_H
#define GX_GR_MESH_H

#include <raylib.h>
#include <GameObject.h>

struct GR_Mesh : ObjectDrawable
{
    Model model;

    GR_Mesh() = default;
    GR_Mesh(Model model) : model(model) {};
    void draw() override
    {
        if(IsShaderReady(model.materials[0].shader))
        {
            model.transform = MatrixRotateXYZ(QuaternionToEuler(componentParent->parentObject->rotation));
            DrawModel(model, componentParent->parentObject->position, 1.0f, WHITE);
        }
        
    }
};
enum ShaderType
{
    LIGHTING,
    UNLIT
};
struct GR_MeshComponent : GameComponent
{
    GR_Mesh *mesh;
    ShaderType shaderType = LIGHTING;
    void OnUpdate() override
    {
        
    }
    void SetShader(Shader *shader )
    {
        mesh->model.materials[0].shader = *shader;// Set shader effect to 3d model
    }
    void SetTexture(const Texture2D& texture)
    {
        mesh->model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture; // Bind texture to model
    }
    void ComponentAddedCallback() override
    {
        parentObject->objectDrawables.push_back(mesh); 
        mesh->componentParent = this;  
    }
    GR_MeshComponent(GR_Mesh *mesh) : mesh(mesh) {};

};
//Raylib Model Container

#endif /* GX_GR_MESH_H */
