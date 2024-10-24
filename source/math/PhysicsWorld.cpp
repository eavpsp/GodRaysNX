#include <PhysicsWorld.h>
#include <raymath.h>    
#include <vector>
#include "ScriptCallbacks.h"
#include <btBulletDynamicsCommon.h>
extern std::vector<PhysicsComponent *> *PhysicsObjects;
static float gravity;
static bool _initialized;
static Vector3 groundPosition = {0,0,0};
static bool useGround = false;
extern EngineCallBacks *engineCallBacks;
Vector3 PhysicsWorld::GetGroundPosition()
{
    return groundPosition;
}
float PhysicsWorld::GetGravity()
{
    return gravity;
}

void PhysicsWorld::SetGravity(float value)
{
    gravity = value;
}

void PhysicsWorld::InitBasic(float gravity, bool ground)
{
    useGround = ground;    
    PhysicsWorld::SetGravity(gravity);
    _initialized = true;
}
void PhysicsWorld::InitBullet()
{
    
        ///-----initialization_start-----
        ///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
        collisionConfiguration = new btDefaultCollisionConfiguration();

        ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
        dispatcher = new btCollisionDispatcher(collisionConfiguration);

        ///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
       overlappingPairCache = new btDbvtBroadphase();

        ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
        solver = new btSequentialImpulseConstraintSolver;

        dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

        dynamicsWorld->setGravity(btVector3(0, -10, 0));

        btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));

        btTransform groundTransform;
        groundTransform.setIdentity();
        groundTransform.setOrigin(btVector3(0, -56, 0));

        btScalar mass(0.);

        //rigidbody is dynamic if and only if mass is non zero, otherwise static
        bool isDynamic = (mass != 0.f);

        btVector3 localInertia(0, 0, 0);
        if (isDynamic)
            groundShape->calculateLocalInertia(mass, localInertia);

        //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
        btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
        btRigidBody* body = new btRigidBody(rbInfo);

        //add the body to the dynamics world
        dynamicsWorld->addRigidBody(body);
        _initialized = true;
}

void PhysicsWorld::ShutdownPhysicsWorld()
{
    _initialized = false;
}
void PhysicsWorld::UpdateBullet()
{
    SetTargetFPS(120);
    dynamicsWorld->stepSimulation(1.f / 120.f, 10);
    SetTargetFPS(60);

}
void PhysicsWorld::UpdateBasic()
{
    if(PhysicsObjects->size() == 0)
    {
        return;
    }
    SetTargetFPS(120);

    //gravity
    for (int i = 0; i < PhysicsObjects->size(); i++)
    {
        if(!PhysicsObjects->at(i)->isKinematic)
        {
            if(PhysicsObjects->at(i)->parentObject->position.y <= PhysicsWorld::GetGroundPosition().y)
            {
                PhysicsObjects->at(i)->parentObject->position.y = PhysicsWorld::GetGroundPosition().y; 
                PhysicsObjects->at(i)->velocity.y = 0; // Implement dampening to reduce speed
                PhysicsObjects->at(i)->groundedYPos = PhysicsObjects->at(i)->parentObject->position.y;
                PhysicsObjects->at(i)->isGrounded = true;
               
            }
            float gravityEffect = PhysicsWorld::GetGravity() * GetFrameTime();

            if(!PhysicsObjects->at(i)->isGrounded)
            {
                PhysicsObjects->at(i)->velocity.y += gravityEffect; // gravity
            }
            
          
            if(PhysicsObjects->at(i)->velocity.y < 0)
            {
                PhysicsObjects->at(i)->parentObject->position.y += PhysicsObjects->at(i)->velocity.y + 0.5f * gravityEffect; // Make the velocity influence the object
            }
            if(PhysicsObjects->at(i)->velocity.x != 0 || PhysicsObjects->at(i)->velocity.y != 0 || PhysicsObjects->at(i)->velocity.z != 0) 
            {
                PhysicsObjects->at(i)->velocity  = Vector3Add(PhysicsObjects->at(i)->velocity , Vector3Add(PhysicsObjects->at(i)->velocity, Vector3Scale(PhysicsObjects->at(i)->acceleration, GetFrameTime())));//apply acceleration
                PhysicsObjects->at(i)->parentObject->position = Vector3Add(PhysicsObjects->at(i)->parentObject->position, Vector3Scale(PhysicsObjects->at(i)->velocity, GetFrameTime()));//apply velocity
                PhysicsObjects->at(i)->velocity = Vector3Scale(PhysicsObjects->at(i)->velocity,  -PhysicsObjects->at(i)->mass * 0.05f);//reduce velocity to simulate drag

            }
        }
    }
    SetTargetFPS(60);

}

void PhysicsWorld::AddForce(PhysicsComponent *obj, Vector3 force)
{
    obj->velocity = force;
    
}
