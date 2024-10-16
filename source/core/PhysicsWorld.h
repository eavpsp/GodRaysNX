#ifndef PHYSICSWORLD_H
#define PHYSICSWORLD_H
#include <PhysicsComponent.h>

class PhysicsWorld
{   

    //add functions for adding forces and other stuff
    public:
        static Vector3 GetGroundPosition();
        static float GetGravity();
        static void SetGravity(float value);
        static void Init(float gravity = -9.8f);
        static void Shutdown();
        static void Update();
        static void AddForce(PhysicsComponent *obj, Vector3 force);
};


#endif