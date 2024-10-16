#include <PhysicsComponent.h>
#include <raymath.h>

extern std::vector<PhysicsComponent *> *PhysicsObjects;
PhysicsComponent::PhysicsComponent()
{
        updateBounds();
        triggerEvents = std::vector<TriggerEvents *>();
        collisionEvents = std::vector<CollisionEvents *>();

}

void PhysicsComponent::updateBounds()
{
     _bounds = {(Vector3){parentObject->position.x - size.x/2,
                        parentObject->position.y -  size.y/2,
                        parentObject->position.z -  size.z/2 },
            (Vector3){ parentObject->position.x +  size.x/2,
                        parentObject->position.y +  size.y/2,
                        parentObject->position.z +  size.z/2 }};
}

PhysicsComponent::~PhysicsComponent()
{
}

void PhysicsComponent::OnUpdate()
{
    updateBounds();
    //loop all objects
    for (size_t i = 0; i < PhysicsObjects->size(); i++)
    {
        if(PhysicsObjects->at(i) == this)
        {
            continue;
        }
        if(PhysicsObjects->at(i)->shape == SPHERE)
        {
            if(CheckCollisionBoxSphere(_bounds , PhysicsObjects->at(i)->parentObject->position, PhysicsObjects->at(i)->radius))
            {
                if(isTrigger)
                {
                        onTrigger(PhysicsObjects->at(i));
                }
                else
                {
                        onCollision(PhysicsObjects->at(i));

                }
            }
        }
        else
        {
            if(CheckCollisionBoxes(_bounds, PhysicsObjects->at(i)->_bounds))
            {
                if(isTrigger)
                {
                        onTrigger(PhysicsObjects->at(i));
                }
                else
                {
                        onCollision(PhysicsObjects->at(i));
                }
            }
        }
        /* code */
       
    }

    
}

void PhysicsComponent::onCollision(PhysicsComponent *other)
{
    if(!isKinematic)
    {
        //check side of collision and prevent movement
        Vector3 displacement = Vector3Subtract(_bounds.max, other->_bounds.min);
        float depth = Vector3Length(displacement);
        if(depth > 0)
        {
            Vector3 normal = Vector3Normalize(displacement);
            Vector3 correction = Vector3Scale(normal, depth);
            parentObject->position = Vector3Subtract(parentObject->position, correction);
        }
    }
    for (size_t i = 0; i < collisionEvents.size(); i++)
    {
        /* code */
        collisionEvents[i]->DoEvent(other);
    }
    
}

void PhysicsComponent::onTrigger(PhysicsComponent *other)
{
    for (size_t i = 0; i < triggerEvents.size(); i++)
    {
        /* code */
        triggerEvents[i]->DoEvent(other);
    }
    
}

void PhysicsComponent::ComponentAddedCallback()
{
    
            PhysicsObjects->push_back(this);
        
}
