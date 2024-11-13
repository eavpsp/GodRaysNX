#ifndef UIDebug_H
#define UIDebug_H
#include "GameOverlays.h"
#include <string>
#include <map>
#include <GameStates.h>
//Used to display a debug menu to be rendered in the debug state

struct OnScreenDebugMenu
{
    bool isOpen;
    virtual void DrawDebugMenu() = 0;
    void OpenMenu()
    {
        isOpen = true;
        ENGINE_STATES::ChangeState(ENGINE_STATES::DEBUG);
    }
    void CloseMenu()
    {
        isOpen = false;
        ENGINE_STATES::ChangeState(ENGINE_STATES::IN_GAME);
    }
    OnScreenDebugMenu() = default;
    virtual ~OnScreenDebugMenu()
    {
        isOpen = false;
    }

};

struct DefaultDebugMenu : public OnScreenDebugMenu//add draba
{
    std::map<std::string, Drawable*> drawables;
    void DrawDebugMenu() override
    {
        for(auto& pair : drawables)
        {
            pair.second->draw();
        }
        
    }
    DefaultDebugMenu()
    {

    }
    void RemoveDrawable(Drawable* drawable)
    {
        for(auto& pair : drawables)
        {
            if(pair.second == drawable)
            {
                drawables.erase(pair.first);
                break;
            }
        }
    }
    void AddDrawable(std::string ItemName, Drawable* drawable)
    {
        drawables.insert(std::pair<std::string, Drawable*>(ItemName, drawable));
    }
};
#endif // UIDebug_H
