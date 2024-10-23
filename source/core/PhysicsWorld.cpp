#include <PhysicsWorld.h>
#include <raymath.h>    
#include <vector>
#include "ScriptCallbacks.h"
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

void PhysicsWorld::Init(float gravity, bool ground)
{
    useGround = ground;    
    PhysicsWorld::SetGravity(gravity);
    _initialized = true;
}

void PhysicsWorld::Shutdown()
{
    _initialized = false;
}

void PhysicsWorld::Update()
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
