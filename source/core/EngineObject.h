#ifndef ENGINEOBJECT_H
#define ENGINEOBJECT_H


#include <raylib.h>
#include <string>
#include <typeinfo>
#include <vector>
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
        Vector3 position, scale;
        Quaternion rotation;
        Matrix transform;
        std::string name;
        bool isActive = true;
        
};


#endif
