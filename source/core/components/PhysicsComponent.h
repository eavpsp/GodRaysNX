#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include <raylib.h>
#include <GameObject.h> 
#include <vector>
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
        bool isKinematic = false;
        bool isTrigger = false;
        CollisionShape shape = CollisionShape::SPHERE;
        float radius = 1;
        Vector3 size = {1,1,1};
        Vector3 velocity = {0,0,0};
        BoundingBox _bounds;
        std::vector<TriggerEvents *> triggerEvents;
        std::vector<CollisionEvents *> collisionEvents;
        PhysicsComponent();
        ~PhysicsComponent();
        void OnUpdate();
        void onCollision(PhysicsComponent *other);
        void onTrigger(PhysicsComponent *other);
        void ComponentAddedCallback();
        


};

#endif // PHYSICSCOMPONENT_H
