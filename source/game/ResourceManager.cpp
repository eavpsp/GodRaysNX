//Resource Trip
//Map String to Resource
#include <map>
#include <raylib.h>
#include <string>
#include "ResourceManager.h"

//FILEPATHS
#define ROBOT_MODEL_PATH "romfs:/models/robot.glb"
#define RAYLIB_MODEL_PATH "romfs:/models/raylib_logo_3d.glb"

#define ENGINE_FONT_PATH "romfs:/resources/mp1m.ttf"

#define ENGINE_LOGO_PATH "romfs:/resources/amw_icon.png"
//---------------------
//TEXTURES
std::map<std::string, std::string> RES_Textures = 
{
  {"ENGINE_LOGO", ENGINE_LOGO_PATH},
};
//FONTS
std::map<std::string, std::string> RES_Fonts = 
{
  {"DEFAULT", ENGINE_FONT_PATH},
};
//MODELS
std::map<int, std::string> RES_Models = 
{
  {_RES::Model_ID::ROBOT_ID, ROBOT_MODEL_PATH},
  {_RES::Model_ID::RAY_LIB_ID, RAYLIB_MODEL_PATH},
};
//MODELS ANIMATIONS
std::map<int, std::string> RES_ModelAnimations = 
{
  {_RES::Model_ID::ROBOT_ID, ROBOT_MODEL_PATH},
};


std::map<_RES::Scenes_Types, std::string> RES_Scenes = 
{
  {_RES::Scenes_Types::ROBOT_SCENE, "romfs:/scenes/robot.grb"},
};


Model _RES::GetModel(int id)
{
   
    return LoadModel(RES_Models[id].c_str());
    
}
