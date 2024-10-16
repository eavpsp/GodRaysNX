#ifndef SCRIPTCALLBACKS_H
#define SCRIPTCALLBACKS_H
constexpr int TARGET_FPS = 60;
constexpr double UPDATE_TICK_DELAY = 1.0 / TARGET_FPS;

#include <vector>
#include <algorithm>
#include <GameObject.h>
#include "../debug/debug.h"
#include <chrono>
extern std::vector<GameObject *> *GameObjects;
extern std::vector<EngineObject *> *GraphicsObjects;


class EngineCallBacks
{
    private:
    double get_detla_time(){
        static auto lastTime = std::chrono::high_resolution_clock::now();
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto deltaTime = std::chrono::duration<double>(currentTime - lastTime).count();
        lastTime = currentTime;
        return deltaTime;
    }
    float updateTicks = 0;
  
    float _deltaTime;
    public:
        static bool IsValidPointer(void* ptr) { return ptr != nullptr; }
        float interpolatedTickTime = (float)(updateTicks/ UPDATE_TICK_DELAY);
        float GetInterpolatedTickTime()
        {
            return interpolatedTickTime;
        }
      
        EngineCallBacks()
        {
           _deltaTime = UPDATE_TICK_DELAY;
       
        };
        ~EngineCallBacks(){};
        
        
       void RunUpdateCallbacks() 
        {

            for(int i = 0; i < GameObjects->size(); i++)
            {
                GameObject* obj = dynamic_cast<GameObject*>(GameObjects->at(i));
                if (obj != nullptr) 
                {
                    obj->onUpdate(); // This will call TestObj::onUpdate() if obj is a TestObj
                }
            
            }

            _deltaTime = get_detla_time();
        }
               
       
       
};


#endif // SCRIPTCALLBACKS_H
