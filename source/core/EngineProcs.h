#ifndef ENGINEPROCS_H
#define ENGINEPROCS_H
#include <GameStates.h>

using ENIGNEPROC = void (*)();
struct EngineProcs
{
    ENGINE_STATES::GAMESTATES state;
    ENIGNEPROC onInit;
    ENIGNEPROC RUN;
    EngineProcs(ENIGNEPROC _onInit, ENIGNEPROC _onUpdate) : onInit(_onInit), RUN(_onUpdate) {}
};

//set up procs for different states
#endif