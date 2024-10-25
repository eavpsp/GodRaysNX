#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include <raylib.h>
#include <GameObject.h> 
#include <vector>
#include <RenderSystem.h>
#include <btBulletDynamicsCommon.h>
/*************************** 
Physics System
------------------
TODO:
Joints
IK
Rolling (Check if on a slope or slant and add velocity)
Bouncing (check velocity going towards object and bounce off in opposite direction)

****************************/
enum CollisionShape
{
    SPHERE,
    BOX
};
class PhysicsComponent;
struct PhysicsEvents
{
    virtual void DoEvent(PhysicsComponent *other) = 0;
};

struct TriggerEvents : public PhysicsEvents
{
    void DoEvent(PhysicsComponent *other) override
    {

    };
};
struct CollisionEvents : public PhysicsEvents
{
    void DoEvent(PhysicsComponent *other) override
    {

    };
};

class PhysicsComponent : public GameComponent
{
    //
    private:
        void updateBounds();
        
    public:
        float groundedYPos=0;
        bool isKinematic = false;
        bool isTrigger = false;
        bool isGrounded = false;
        CollisionShape shape = CollisionShape::SPHERE;
        float radius = 0;
        bool canBounce = false;
        bool canRoll = false;
        Vector3 size = {1,1,1};
        Vector3 velocity = {0,0,0};
        Vector3 acceleration = {0,0,0};
        bool useGravity = true;
        float angularVelocity = 0;
        float mass = 1.0f;
        MightyBoundingBox _bounds;
        std::vector<TriggerEvents *> triggerEvents;
        std::vector<CollisionEvents *> collisionEvents;
        PhysicsComponent(float mass, Vector3 _size, bool _isTrig, bool _isKinematic = false) 
        {
            this->mass = mass;
            shape = CollisionShape::BOX;
            updateBounds();
            triggerEvents = std::vector<TriggerEvents *>();
            collisionEvents = std::vector<CollisionEvents *>();
            this->isKinematic = _isKinematic;
            this->isTrigger = _isTrig;
            this->size = _size;
            
        };
        PhysicsComponent(float mass, float radius, bool _isTrig, bool _isKinematic = false) 
        {   
            this->mass = mass;

            shape = CollisionShape::SPHERE;
            triggerEvents = std::vector<TriggerEvents *>();
            collisionEvents = std::vector<CollisionEvents *>();
            this->radius = radius;
            this->isKinematic = _isKinematic;
            this->isTrigger = _isTrig;
        };
        ~PhysicsComponent()
        {
            triggerEvents.clear();
            collisionEvents.clear();
        };
        void SetAngularVelocity(float angularVelocity, Vector3 axis);
        void OnUpdate();
        void onCollision(PhysicsComponent *other);
        void onTrigger(PhysicsComponent *other);
        void ComponentAddedCallback();
        


};


class BulletPhysicsComponent : public GameComponent
{
    btAlignedObjectArray<btCollisionShape*> collisionShapes;//Bullet Shapes Vector
    btTransform *transform;
    btScalar mass;
    btVector3 localInertia;
    btDefaultMotionState* myMotionState;
    btRigidBody* body;
    void ComponentAddedCallback() override
    {
        return;
    }
    //ref to gameobject
   
   
    void BindMatrix()
    {       

        myMotionState->getWorldTransform(*transform); 

        //bullet and open gl postions are inverted, might fix in shader
        parentObject->position = Vector3{transform->getOrigin().x(), transform->getOrigin().y(), transform->getOrigin().z()};//fine
        btQuaternion rot = transform->getRotation();
         Vector3 axis = {
            rot.getAxis().getX(),
            rot.getAxis().getY(),
            rot.getAxis().getZ()
        };
        float angle = rot.getAngle() * RAD2DEG;

        parentObject->rotation = QuaternionFromAxisAngle(axis, angle);//fine
    }
    public:
/**
 * Constructor for BulletPhysicsComponent.
 * @param pos The initial position of the rigid body.
 * @param rot The initial rotation of the rigid body.
 * @param mass The mass of the rigid body (0 for static, non-zero for dynamic).
 * @param localInertia The local inertia of the rigid body.
 * @param shape The collision shape of the rigid body.
 */
        BulletPhysicsComponent(Vector3 pos, Quaternion rot, float mass, btCollisionShape* shape);
       
        ~BulletPhysicsComponent(){};

        void OnUpdate() override
        {
            if(!parentObject->isActive || !isActive)
            {
                return;
            }
            BindMatrix();
        }
};

#endif // PHYSICSCOMPONENT_H
