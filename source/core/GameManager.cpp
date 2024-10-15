#include <GameManager.h>
#include "../debug/debug.h"
#include <raylib.h>
#include <raygui.h>

Font guiFont;

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
        guiFont = LoadFont("romfs:/resources/mp1m.ttf");//set var for game fonts
        texture = LoadTexture("romfs:/resources/amw_icon.png");  //texture test
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
   
}
 
void GameManager::renderLoop()
{
 
         BeginDrawing();

            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

            if (GuiTextBox((Rectangle){ 25, 215, 125, 30 }, "Hello", 64, false));

            DrawTexture(texture, screenWidth/2 - texture.width/2, screenHeight/2 - texture.height/2, WHITE);

            DrawTextEx(guiFont, "this IS a texture!", Vector2{360, 650}, 40, 40/2,GRAY);

        EndDrawing();
    
}

bool GameManager::Running(){
     return _running;
}

