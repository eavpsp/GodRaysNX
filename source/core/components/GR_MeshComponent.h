#ifndef GX_GR_MESH_H
#define GX_GR_MESH_H

#include <raylib.h>
#include <GameObject.h>
#include <raymath.h>
struct GR_Mesh : ObjectDrawable
{
    Model model;
    Model activeMesh;
    Model *LOD_Meshes;
    GR_Mesh() = default;
    GR_Mesh(Model model) : model(model) {
        activeMesh = model;
       
    };
    void DrawLODMesh(int index)
    {
        activeMesh = LOD_Meshes[index];
    }
    void DrawBaseMesh()
    {
        activeMesh = model;
    }
    void SetLOD(Model *LOD_Meshes) { this->LOD_Meshes = LOD_Meshes; }
    void draw() override
    {
        
        DrawModel(activeMesh, componentParent->parentObject->position, componentParent->parentObject->scale.y, WHITE);//update to draw ex
    
        
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

    //LOD vector
    ShaderType shaderType = LIGHTING;
    void OnUpdate() override
    {
           mesh->activeMesh.transform = MatrixMultiply(MatrixIdentity(), MatrixRotateXYZ((Vector3){ DEG2RAD*parentObject->rotation.x, DEG2RAD*parentObject->rotation.y, DEG2RAD*parentObject->rotation.z }));

    }
   
    void SetShader(Shader shader )
    {
        for (size_t i = 0; i < mesh->activeMesh.materialCount; i++)
        {
            mesh->activeMesh.materials[i].shader = shader;// Set shader effect to 3d model
        }
        
      
    }
   
    void SetTexture(const Texture2D& texture)
    {
        mesh->activeMesh.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture; // Bind texture to model
    }
   
    void ComponentAddedCallback() override
    {
        parentObject->objectDrawables.push_back(mesh); 
        mesh->componentParent = this;  
    }
    GR_MeshComponent(GR_Mesh *mesh) : mesh(mesh) 
    {

        Matrix rotMatrix = MatrixRotateXYZ({parentObject->rotation.x, parentObject->rotation.y, parentObject->rotation.z});
        mesh->activeMesh.transform = rotMatrix;
    };

};
//Raylib Model Container

#endif /* GX_GR_MESH_H */
