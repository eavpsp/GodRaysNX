//Resource Trip
//Map String to Resource
#include <map>
#include <raylib.h>
#include <string>
#include "ResourceManager.h"
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
std::map<std::string, std::string> RES_Models = 
{
  {"ROBOT", "romfs:/models/robot.glb"},
  {"RAYLIB_LOGO", "romfs:/models/raylib_logo_3d.glb"},
};
//MODELS ANIMATIONS
std::map<std::string, std::string> RES_ModelAnimations = 
{
  {"ROBOT", "romfs:/models/robot.glb"},
};

Model _RES::GetModel(std::string name)
{
   
    return LoadModel(RES_Models[name].c_str());
    
}
