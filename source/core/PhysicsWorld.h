#ifndef PHYSICSWORLD_H
#define PHYSICSWORLD_H
#include <PhysicsComponent.h>

class PhysicsWorld
{   
   
    //add functions for adding forces and other stuff
    public:
    //ground game object
    //Raycast for ground
        static Vector3 GetGroundPosition();
        static float GetGravity();
        static void SetGravity(float value);
        static void Init(float gravity = -9.8f, bool ground = true);
        static void Shutdown();
        static void Update();
        static void AddForce(PhysicsComponent *obj, Vector3 force);

        static RayCollision ShootRayCollision(Vector3 pos, Vector3 dir, BoundingBox box)
        {
            Ray ray = {pos, dir};
            return GetRayCollisionBox(ray, box);
        }
        static RayCollision ShootRayMesh(Vector3 pos, Vector3 dir, Mesh mesh, Matrix transform)
        {
            Ray ray = {pos, dir};
            return GetRayCollisionMesh(ray, mesh, transform);
        }

        static void DebugDrawRay(Vector3 pos, Vector3 dir, Color color)
        {
            Ray ray = {pos, dir};
            DrawRay(ray, color);
        }
};




#endif