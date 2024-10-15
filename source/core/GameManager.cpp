#include <GameManager.h>
#include "../debug/debug.h"
#include <raylib.h>
#include <raygui.h>
#include <map>
#include <string>
#include <vector>
#include <GameObject.h>
#include <GameOverlays.h>
#include "ScriptCallbacks.h"
extern std::map<std::string, std::string> RES_Textures;
extern std::map<std::string, std::string> RES_Fonts;
extern std::vector<GameObject *> *GameObjects;
Font guiFont;
Camera camera;
    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)
Texture2D texture;        // Texture loading
const int screenWidth = 1280;
const int screenHeight = 720;

GameManager::GameManager(bool running) : _running{running}
{

}

GameManager::~GameManager()
{
      UnloadTexture(texture); 
}
GameManager& GameManager::getGameManager()
{
    static GameManager *gameManager = nullptr;
    if(gameManager == nullptr)
    {

        gameManager = new GameManager(true);
        InitWindow(screenWidth, screenHeight, "raylib [textures] example - texture loading and drawing");
        if (!IsWindowReady())
        {
            debugLog("\x1b[16;25HError Creating Window!");
        }
        SetTargetFPS(60);
        camera = { 0 };
        camera.position = (Vector3){ 6.0f, 6.0f, 6.0f };    // Camera position
        camera.target = (Vector3){ 0.0f, 2.0f, 0.0f };      // Camera looking at point
        camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
        camera.fovy = 45.0f;                                // Camera field-of-view Y
        camera.projection = CAMERA_PERSPECTIVE;   
        guiFont = LoadFont(RES_Fonts["DEFAULT"].c_str());//set var for game fonts
        texture = LoadTexture(RES_Textures["ENGINE_LOGO"].c_str());  //texture test
        debugLog("Made Game Manager!");
    }
    return *gameManager;
}

void GameManager::destroyGameManager()
{
        GameManager *gameManager = &getGameManager();

        if(gameManager != nullptr)
        {
            debugLog("\x1b[16;20HDestroyed GameManager!");
            delete gameManager;

        }
    }

void GameManager::runGameLoop()
{
    UpdateCamera(&camera, CAMERA_ORBITAL);
}
 
void GameManager::renderLoop()
{
 
         BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

            //Loop gameobjects
            for (size_t i = 0; i < GameObjects->size(); i++)
            {
                /* code */
                if(EngineCallBacks::IsValidPointer(GameObjects->at(i)))
                GameObjects->at(i)->Draw();
            }   
            DrawGrid(10, 1.0f);
            

            EndMode3D();
            if (GuiTextBox((Rectangle){ 25, 215, 125, 30 }, "Hello", 64, false));

          //  DrawTexture(texture, screenWidth/2 - texture.width/2, screenHeight/2 - texture.height/2, WHITE);
         
           // DrawTextEx(guiFont, "this IS a texture!", Vector2{360, 650}, 40, 40/2,GRAY);

        EndDrawing();
    
}

bool GameManager::Running(){
     return _running;
}

