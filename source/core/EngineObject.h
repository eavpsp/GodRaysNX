#ifndef ENGINEOBJECT_H
#define ENGINEOBJECT_H


#include <raylib.h>
#include <string>
#include <typeinfo>
#include <vector>

struct ObjectDrawable;

class EngineObject
{
    //private funcs onUpdate, onDraw onInit onDestroy
    //implement component handling
    private:

        virtual void onInit() = 0;
        virtual void onDestroy() = 0;
        template <typename T>
        const char* get_type_name() {
            return typeid(T).name();
        }   
    public:
        virtual void onUpdate() = 0;
        virtual void onDraw() = 0;
        EngineObject();
        virtual ~EngineObject(){};
        std::vector<ObjectDrawable*> objectDrawables;
        Vector3 position;
        float scale = 1.0f;
        Vector3 forward = {0, 0, 1};
        Vector3 upVector = {0, 1, 0};
        Quaternion rotation;
        Matrix transform;
        std::string name;
        bool isActive = true;
        
};


#endif
