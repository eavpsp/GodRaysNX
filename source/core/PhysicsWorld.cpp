#include <PhysicsWorld.h>
#include <raymath.h>    
#include <vector>
extern std::vector<PhysicsComponent *> *PhysicsObjects;
static float gravity;
static bool _initialized;
static Vector3 groundPosition = {0,0,0};
static bool useGround = false;
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
    //gravity
    for (int i = 0; i < PhysicsObjects->size(); i++)
    {
        if(!PhysicsObjects->at(i)->isKinematic)
        {
           if(PhysicsObjects->at(i)->parentObject->position.y > PhysicsWorld::GetGroundPosition().y)
           {
                PhysicsObjects->at(i)->isGrounded = false;  
           }
            PhysicsObjects->at(i)->velocity  = Vector3Add(PhysicsObjects->at(i)->velocity , Vector3Add(PhysicsObjects->at(i)->velocity, Vector3Scale(PhysicsObjects->at(i)->acceleration, GetFrameTime() / PhysicsObjects->at(i)->mass)));
            PhysicsObjects->at(i)->parentObject->position = Vector3Add(PhysicsObjects->at(i)->parentObject->position, Vector3Scale(PhysicsObjects->at(i)->velocity, GetFrameTime()));

            if(PhysicsObjects->at(i)->parentObject->position.y == PhysicsWorld::GetGroundPosition().y && useGround)
            {
                PhysicsObjects->at(i)->velocity = Vector3Zero(); // Implement dampening to reduce speed
                PhysicsObjects->at(i)->isGrounded = true;
               
            }
            if(PhysicsObjects->at(i)->parentObject->position.y < PhysicsWorld::GetGroundPosition().y && useGround)
            {
                PhysicsObjects->at(i)->parentObject->position = Vector3{PhysicsObjects->at(i)->parentObject->position.x, PhysicsWorld::GetGroundPosition().y, PhysicsObjects->at(i)->parentObject->position.z}; 
                PhysicsObjects->at(i)->velocity.y = 0; // Implement dampening to reduce speed
                PhysicsObjects->at(i)->isGrounded = true;

               
            }

            float gravityEffect = PhysicsWorld::GetGravity() * GetFrameTime();
            PhysicsObjects->at(i)->velocity.y += gravityEffect; // Make the velocity influence the object
            
            //check if the object is accelerating up
            if(PhysicsObjects->at(i)->velocity.y < 0)
            {
                PhysicsObjects->at(i)->parentObject->position.y += PhysicsObjects->at(i)->velocity.y + 0.5f * gravityEffect; // Make the velocity influence the object
            }
            PhysicsObjects->at(i)->velocity = Vector3Scale(PhysicsObjects->at(i)->velocity,  -PhysicsObjects->at(i)->mass * 0.05f);
        }
    }
}

void PhysicsWorld::AddForce(PhysicsComponent *obj, Vector3 force)
{
    obj->velocity = force;
    
}
