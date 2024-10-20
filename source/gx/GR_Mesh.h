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
        DrawModel(model, componentParent->parentObject->position, 1.0f, WHITE);
    }
};

struct GR_MeshComponent : GameComponent
{
    GR_Mesh *mesh;
    void OnUpdate() override
    {
        
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
