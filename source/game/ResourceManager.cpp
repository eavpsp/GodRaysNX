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


Model _RES::GetModel(int id)
{
   if(id == -1)
   {
       return Model();
   }
    return LoadModel(RES_Models[id].c_str());
    
}
