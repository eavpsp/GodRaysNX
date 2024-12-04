#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include <raylib.h>
#include <GameObject.h> 
#include <vector>
#include <RenderSystem.h>
#include <btBulletDynamicsCommon.h>

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
struct BulletPhysicsComponent;
struct BulletPhysicsEvents
{
    virtual void DoEvent(BulletPhysicsComponent *other) = 0;
    virtual ~BulletPhysicsEvents() {}
};
struct BP_TriggerEvents : public BulletPhysicsEvents
{
    void DoEvent(BulletPhysicsComponent *other) override
    {

    };
    ~BP_TriggerEvents() {}
};
struct BP_CollisionEvents : public BulletPhysicsEvents   
{
    void DoEvent(BulletPhysicsComponent *other) override
    {

    };
    ~BP_CollisionEvents() {}
};

struct BulletPhysicsComponent : public GameComponent
{
    bool isTrigger = false;
    bool isKinematic = false;
    bool debugCollider = false;
    btCollisionObject *collisionObject = nullptr;
    btCollisionShape *shape = nullptr;
    btTransform *transform = nullptr;
    btScalar mass;
    btVector3 localInertia;
    btDefaultMotionState* myMotionState = nullptr;
    btRigidBody* body = nullptr;
    std::vector<BP_TriggerEvents *> triggerEvents = {};
    std::vector<BP_CollisionEvents *> collisionEvents = {};
    std::vector<BP_TriggerEvents *> triggerExitEvents = {};
    std::vector<BP_CollisionEvents *> collisionExitEvents = {};
    void DrawCollider();
    void ComponentAddedCallback() override
    {
        return;
    }
    int GetShapeType()
    {
        return shape->getShapeType();
    }
    //ref to gameobject
    bool CheckCollision(btCollisionObject* other);
    
    void BindMatrix()
    {       

        myMotionState->getWorldTransform(*transform); 
        parentObject->position = Vector3{transform->getOrigin().x(), transform->getOrigin().y(), transform->getOrigin().z()};
        btQuaternion rot = transform->getRotation();
         Vector3 axis = {
            rot.getAxis().getX(),
            rot.getAxis().getY(),
            rot.getAxis().getZ()
        };
        float angle = rot.getAngle() * RAD2DEG;

        parentObject->rotation = QuaternionFromAxisAngle(axis, angle);
    }
    public:

 
        BulletPhysicsComponent(Vector3 pos, Quaternion rot, float mass, btCollisionShape* shape);
       
        ~BulletPhysicsComponent(){};
        void onCollision(BulletPhysicsComponent *other);
        void onTrigger(BulletPhysicsComponent *other);
        void onCollisionExit(BulletPhysicsComponent *other);
        void onTriggerExit(BulletPhysicsComponent *other);
        void OnUpdate() override;
        
};


struct Physics2DComponent : public GameComponent
{
    bool isTrigger = false;
    bool isKinematic = false;
    bool useGravity = true;
    float mass = 1.0f;
    enum CollisionShape
    {
        SPHERE,
        BOX
    };
    CollisionShape shape = CollisionShape::BOX;
    float radius = 0;
    Vector2 velocity = {0,0};
    Rectangle rect;
    void onCollision(Physics2DComponent *other);
    void onTrigger(Physics2DComponent *other);
    void ComponentAddedCallback() override;
    void OnUpdate() override;
    Physics2DComponent(float mass, float radius, bool _isTrig = false, bool _isKinematic = false);
    Physics2DComponent(float mass, Vector2 _size, bool _isTrig = false, bool _isKinematic = false);
    ~Physics2DComponent() {}

};

#endif // PHYSICSCOMPONENT_H
