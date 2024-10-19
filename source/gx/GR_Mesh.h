#ifndef GX_GR_MESH_H
#define GX_GR_MESH_H

#include <raylib.h>

struct GR_Mesh
{
    Model model;

    GR_Mesh() = default;
    GR_Mesh(Model model) : model(model) {}
};
//Raylib Model Container

#endif /* GX_GR_MESH_H */
