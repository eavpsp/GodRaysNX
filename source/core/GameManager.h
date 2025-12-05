#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H
#include <GamePad.h>
#include <time.h>
#include <GameStates.h>
#include <ResourceManager.h>
extern std::map<int, std::string> RES_Fonts;
struct GameConfig
{
    const int screenWidth = 1280;
    const int screenHeight = 720;
};

class GameManager
{
    private:
        bool _running;
        ~GameManager();
    public:
    void runGameLoop();//add states for pause and other menus
    void SceneInit();
    void GameLoop();
    bool Running();
    void Init();
    void Quit()
    {
        _running = false;
    }
    static void destroyGameManager();
    GameConfig gameConfig;
    GamePads gamePads;
    GameManager(bool running);
};


#endif