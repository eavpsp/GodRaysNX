//Resource Trip
//Map String to Resource
#include <map>
#include <raylib.h>
#include <string>
#include "ResourceManager.h"


//---------------------
//TEXTURES
std::map<std::string, std::string> RES_Textures = 
{
  {"ENGINE_LOGO", "romfs:/resources/amw_icon.png"},
};
//FONTS
std::map<std::string, std::string> RES_Fonts = 
{
  {"DEFAULT", "romfs:/resources/mp1m.ttf"},
};
//MODELS
std::map<int, std::string> RES_Models = 
{
  {_RES::Model_ID::ROBOT_ID, "romfs:/models/robot.glb"},
  {_RES::Model_ID::RAY_LIB_ID, "romfs:/models/raylib_logo_3d.glb"},
};
//MODELS ANIMATIONS
std::map<int, std::string> RES_ModelAnimations = 
{
  {_RES::Model_ID::ROBOT_ID, "romfs:/models/robot.glb"},
};

//GAME SCENES
std::map<_RES::Scenes_Types, std::string> RES_Scenes = 
{
  {_RES::Scenes_Types::ROBOT_SCENE, "romfs:/scenes/robot.grb"},
  {_RES::Scenes_Types::TEST_SCENE, "romfs:/scenes/godRayMatrix.grb"},
};
//GAME AUDIO 
std::map<int, std::string> RES_AudioFiles = 
{
  {0, "romfs:/audio/test.wav"},
};
//GAME SHADERS
std::map<int, std::pair<std::string, std::string>> RES_Shaders = 
{
  {_RES::ShaderFiles::LIGHT, {"romfs:/shaders/glsl100/lighting.vs", "romfs:/shaders/glsl100/lighting.fs"}},
};

// Loads and returns a lighting shader with predefined settings.
// This function initializes a shader using the vertex and fragment shader files specified in the RES_Shaders map.
// It sets the shader's "viewPos" and "ambient" uniform locations and assigns an ambient light value.
// Returns the configured Shader object.
Shader _RES::GetLightShader()
{
    Shader shader = LoadShader(RES_Shaders[_RES::ShaderFiles::LIGHT].first.c_str(), RES_Shaders[_RES::ShaderFiles::LIGHT].second.c_str());
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
    int ambientLoc = GetShaderLocation(shader, "ambient");
    SetShaderValue(shader, ambientLoc, (float[4]){ 0.2f, 0.2f, 0.2f, 1.0f }, SHADER_UNIFORM_VEC4);
    return shader;
}
Model _RES::GetModel(int id)
{
   if(id == -1)
   {
       return Model();
   }
    return LoadModel(RES_Models[id].c_str());
    
}
