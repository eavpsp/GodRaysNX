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


void SetProc(EngineProcs* proc);
void RunProc(EngineProcs* proc);

#endif