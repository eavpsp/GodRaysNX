#ifndef GAMESTATES_H
#define GAMESTATES_H
///Cutscene, Menu, InGame
namespace ENGINE_STATES
{
   
    enum GAMESTATES
    {
        CUTSCENE,
        VIDEO,
        MENU,
        IN_GAME,
        IDLE,
        LOADING
    };
    static GAMESTATES currentState = GAMESTATES::IN_GAME;

    void ChangeState(GAMESTATES state);
    
    GAMESTATES GetState();
    

} 


#endif // GAMESTATES_H
