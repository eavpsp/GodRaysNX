#ifndef PHYSICSWORLD_H
#define PHYSICSWORLD_H
#include <PhysicsComponent.h>

class PhysicsWorld
{   

    public:
        static Vector3 GetGroundPosition();
        static float GetGravity();
        static void SetGravity(float value);
        static void Init(float gravity = -9.8f);
        static void Shutdown();
        static void Update();
};


#endif