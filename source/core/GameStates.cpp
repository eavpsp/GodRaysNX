#include<GameStates.h>

void ENGINE_STATES::ChangeState(GAMESTATES state)
{
        currentState = state;
    
}

ENGINE_STATES::GAMESTATES ENGINE_STATES::GetState()
{
     
        return currentState;
    
}
