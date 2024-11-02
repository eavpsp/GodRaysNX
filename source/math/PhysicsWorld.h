#ifndef PHYSICSWORLD_H
#define PHYSICSWORLD_H
#include <PhysicsComponent.h>
#include <btBulletDynamicsCommon.h>

class PhysicsWorld
{   

    public:
    //BASIC
         Vector3 GetGroundPosition();
         float GetGravity();
         void SetGravity(float value);
         void InitBasic(float gravity = -9.8f, bool ground = true);
         void UpdateBasic();
         void AddForce(PhysicsComponent *obj, Vector3 force);
    ///BULLET
        btDiscreteDynamicsWorld*  dynamicsWorld;
        btSequentialImpulseConstraintSolver* solver;
        btBroadphaseInterface* overlappingPairCache;
        btDefaultCollisionConfiguration* collisionConfiguration;
        btCollisionDispatcher* dispatcher;
        void DoPhysicsSimulation();
        void AddForceBullet(BulletPhysicsComponent *obj, Vector3 force);
        void InitBullet();
        void UpdateBullet();
    //BOTH
        void ShutdownPhysicsWorld();
    PhysicsWorld()
    {};
};

namespace PhysicsUtil
{
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
       static RayCollision ShootRayMeshFromScreen(Mesh mesh, Camera3D camera, Matrix transform)
        {
            Vector2 mousePosition = GetMousePosition();
            Ray ray = GetScreenToWorldRay(mousePosition, camera);
            return GetRayCollisionMesh(ray, mesh, transform);
        }
        
}



#endif