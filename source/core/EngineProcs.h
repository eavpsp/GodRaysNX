#ifndef ENGINEPROCS_H
#define ENGINEPROCS_H
#include <GameStates.h>

//current proc for each state
//funcs to change the proc and the state

using ENIGNEPROC = void (*)();
struct EngineProcs
{
    ENGINE_STATES::GAMESTATES state;
    ENIGNEPROC onInit;
    ENIGNEPROC RUN;
    EngineProcs() = default;
    virtual ~EngineProcs() = default; 
};
struct CUTSCENE_PROC : EngineProcs
{
    CUTSCENE_PROC(ENIGNEPROC _onInit, ENIGNEPROC _onUpdate)
    {
        this->onInit = _onInit;
        this->RUN = _onUpdate;
        state = ENGINE_STATES::CUTSCENE;
    }
};
struct VIDEO_PROC : EngineProcs
{
    VIDEO_PROC(ENIGNEPROC _onInit, ENIGNEPROC _onUpdate)
    {
        this->onInit = _onInit;
        this->RUN = _onUpdate;
        state = ENGINE_STATES::VIDEO;
    }
};
struct MENU_PROC : EngineProcs
{
    MENU_PROC(ENIGNEPROC _onInit, ENIGNEPROC _onUpdate)
    {
        this->onInit = _onInit;
        this->RUN = _onUpdate;
        state = ENGINE_STATES::MENU;
    }
};

struct IN_GAME_PROC : EngineProcs
{
    IN_GAME_PROC(ENIGNEPROC _onInit, ENIGNEPROC _onUpdate)
    {
        this->onInit = _onInit;
        this->RUN = _onUpdate;
        state = ENGINE_STATES::IN_GAME;
    }
};

struct IDLE_PROC : EngineProcs
{
    IDLE_PROC(ENIGNEPROC _onInit, ENIGNEPROC _onUpdate)
    {
        this->onInit = _onInit;
        this->RUN = _onUpdate;
        state = ENGINE_STATES::IDLE;
    }
};

struct LOADING_PROC : EngineProcs
{
    LOADING_PROC(ENIGNEPROC _onInit, ENIGNEPROC _onUpdate)
    {
        this->onInit = _onInit;
        this->RUN = _onUpdate;
        state = ENGINE_STATES::LOADING;
    }
};

struct BOOT_PROC : EngineProcs
{
    BOOT_PROC(ENIGNEPROC _onInit, ENIGNEPROC _onUpdate)
    {
        this->onInit = _onInit;
        this->RUN = _onUpdate;
        state = ENGINE_STATES::BOOT;
    }
};

struct TEST_PROC : EngineProcs
{
    TEST_PROC(ENIGNEPROC _onInit, ENIGNEPROC _onUpdate)
    {
        this->onInit = _onInit;
        this->RUN = _onUpdate;
        state = ENGINE_STATES::TEST;
    }
};

struct DEBUG_PROC : EngineProcs
{
    DEBUG_PROC(ENIGNEPROC _onInit, ENIGNEPROC _onUpdate)
    {
        this->onInit = _onInit;
        this->RUN = _onUpdate;
        state = ENGINE_STATES::DEBUG;
    }
};

CUTSCENE_PROC* cutsceneProc;
VIDEO_PROC* videoProc;
MENU_PROC* menuProc;
IN_GAME_PROC* inGameProc;
IDLE_PROC* idleProc;
LOADING_PROC* loadingProc;
BOOT_PROC* bootProc;
TEST_PROC* testProc;
DEBUG_PROC* debugProc;

void SetProc(EngineProcs* proc)
{
    switch (proc->state)
    {
    case ENGINE_STATES::VIDEO:
        videoProc = (VIDEO_PROC*)proc;
        break;
    case ENGINE_STATES::CUTSCENE:
        cutsceneProc = (CUTSCENE_PROC*)proc;
        break;
    case ENGINE_STATES::MENU:
        menuProc = (MENU_PROC*)proc;
        break;
    case ENGINE_STATES::IN_GAME:
        inGameProc = (IN_GAME_PROC*)proc;
        break;
    case ENGINE_STATES::IDLE:
        idleProc = (IDLE_PROC*)proc;
        break;
    case ENGINE_STATES::LOADING:
        loadingProc = (LOADING_PROC*)proc;
        break;
    case ENGINE_STATES::BOOT:
        bootProc = (BOOT_PROC*)proc;
        break;
    case ENGINE_STATES::TEST:
        testProc = (TEST_PROC*)proc;
        break;
    case ENGINE_STATES::DEBUG:
        debugProc = (DEBUG_PROC*)proc;
        break;
    
    default:
        break;
    }
}
void RunProc(EngineProcs* proc)
{
    ENGINE_STATES::ChangeState(proc->state);
    SetProc(proc);
    proc->onInit();
}
#endif