#include <EngineProcs.h>

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
    if(proc->onInit != nullptr)
    {
        proc->onInit();
    }
}