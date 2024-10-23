#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
#include <map>
#include <string>
#include <raylib.h>
extern std::map<std::string, std::string> RES_Fonts;
extern std::map<std::string, std::string> RES_Textures;
extern std::map<int, std::string> RES_Models;


namespace _RES
{
    static Model GetModel(int id);
    static Shader GetLightShader(); 
    enum Scenes_Types
    {
        ROBOT_SCENE = 0,
        TEST_SCENE
    };
    enum ShaderFiles
    {
        LIGHT = 0,
    };
    enum Model_ID
    {
        ROBOT_ID = 0,
        RAY_LIB_ID
    };

}
#endif