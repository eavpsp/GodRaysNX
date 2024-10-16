#include <PhysicsComponent.h>
#include <raymath.h>
#include "../debug/debug.h" 
#include <PhysicsWorld.h>
extern std::vector<PhysicsComponent *> *PhysicsObjects;
void PhysicsComponent::updateBounds()
{
     _bounds = {(Vector3){parentObject->position.x - size.x/2,
                        parentObject->position.y -  size.y/2,
                        parentObject->position.z -  size.z/2 },
            (Vector3){ parentObject->position.x +  size.x/2,
                        parentObject->position.y +  size.y/2,
                        parentObject->position.z +  size.z/2 }};
}

void PhysicsComponent::OnUpdate()
{
    if(radius == 0)
    {
         updateBounds();

    }
    if(isGrounded)
    {   
        if(canRoll)
        {
            //check if there is a slope
            //roll in the direction of gravity
            Vector3 direction = Vector3Normalize(Vector3Subtract(Vector3{0,PhysicsWorld::GetGravity(),0}, parentObject->position));
            velocity = Vector3Add(velocity, Vector3Scale(direction, 50 * GetFrameTime() / mass));
            parentObject->rotation = QuaternionFromVector3ToVector3(QuaternionToEuler(parentObject->rotation), direction);
            
            
        }
    }
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
       if(other->isKinematic)
       {
           if(velocity.y + parentObject->position.y < other->parentObject->position.y + other->size.y/2)
           {
               velocity = Vector3Scale(velocity, -1);
               return;
           }
           if(velocity.y + parentObject->position.y > other->parentObject->position.y + other->size.y/2)
           {
               velocity = Vector3Scale(velocity, -1);
               return;
           }
           if (velocity.x + parentObject->position.x < other->parentObject->position.x + other->size.x/2)
           {
               velocity = Vector3Scale(velocity, -1);
               return;
           }
           if (velocity.x + parentObject->position.x > other->parentObject->position.x + other->size.x/2)
           {
               velocity = Vector3Scale(velocity, -1);
               return;
           }
           if (velocity.z + parentObject->position.z < other->parentObject->position.z + other->size.z/2)
           {
               velocity = Vector3Scale(velocity, -1);
               return;
           }
           if (velocity.z + parentObject->position.z > other->parentObject->position.z + other->size.z/2)
           {
               velocity = Vector3Scale(velocity, -1);
               return;
           }
       }
       else
       {
            float j = -(1 + 0.15f) * (velocity.x * (other->parentObject->position.x - parentObject->position.x) +
                                    velocity.y * (other->parentObject->position.y - parentObject->position.y) +
                                    velocity.z * (other->parentObject->position.z - parentObject->position.z)) /
                       (mass + other->mass);

            Vector3 impulse = Vector3Scale(Vector3Subtract(parentObject->position, other->parentObject->position),
                                           j);

            velocity = Vector3Subtract(velocity, Vector3Scale(impulse, 1 / (mass * 2)));
            other->velocity = Vector3Add(other->velocity, Vector3Scale(impulse, 1 / (other->mass * 2)));
            
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
