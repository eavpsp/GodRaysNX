#ifndef LIGHTCOMPONENT_H
#define LIGHTCOMPONENT_H

#include <GameObject.h>
#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"
#include <raylib.h>

struct LightComponent : public GameComponent
{
    Light light;
    Camera *camera;
    Shader shader;
    void OnUpdate() override 
    {
        float cameraPos[3] = { camera->position.x, camera->position.y, camera->position.z };
        SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);
    };
    void ComponentAddedCallback() override {};

    LightComponent(Color lightColor, Vector3 position, Vector3 target, int type = LIGHT_POINT)
    {
        shader = LoadShader(TextFormat("romfs:/shaders/glsl%i/lighting.vs", 330),
                               TextFormat("romfs:/shaders/glsl%i/lighting.fs", 330));
    // Get some required shader locations
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
    // Ambient light level (some basic lighting)
    int ambientLoc = GetShaderLocation(shader, "ambient");
    SetShaderValue(shader, ambientLoc, (float[4]){ 0.1f, 0.1f, 0.1f, 1.0f }, SHADER_UNIFORM_VEC4);
        light = CreateLight(type, position, target, lightColor, shader);
    }
    ~LightComponent() {}
};

//Light Object Container component


#endif // LIGHTCOMPONENT_H
