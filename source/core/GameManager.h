#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <time.h>
#include <GameStates.h>
#include <ResourceManager.h>
#include <RenderSystem.h>


class GameManager
{
    private:
        bool _running;
        GameManager(bool running);
        ~GameManager();
    public:
    void runGameLoop();//add states for pause and other menus
    void SceneInit();
    void GameLoop();
    bool Running();
    // RenderSystem *_renderSystem;
    // ResourceManager *_resourceManager;
    static GameManager& getGameManager();
    static void destroyGameManager();


};


#endif