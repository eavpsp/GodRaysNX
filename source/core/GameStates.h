#ifndef GAMESTATES_H
#define GAMESTATES_H


///Cutscene, Menu, InGame

namespace ENGINE_STATES
{
   
    enum GAMESTATES
    {
        CUTSCENE,
        MENU,
        IN_GAME,
        IDLE
    };
    static GAMESTATES currentState = GAMESTATES::IN_GAME;

    void ChangeState(GAMESTATES state)
    {
        currentState = state;
    }
    GAMESTATES GetState()
    {
        return currentState;
    }
    

} 


#endif // GAMESTATES_H
