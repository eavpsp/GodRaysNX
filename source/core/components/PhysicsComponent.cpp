#include <PhysicsComponent.h>
#include <raymath.h>
#include "../debug/debug.h" 
#include <PhysicsWorld.h>
#include <RenderSystem.h>   
extern std::vector<PhysicsComponent *> *PhysicsObjects;
void PhysicsComponent::updateBounds()//
{
    
    _bounds.UpdateCorners(parentObject->position, size);
    _bounds.Rotate(parentObject->rotation.x, parentObject->rotation.y, parentObject->rotation.z);
}

void PhysicsComponent::OnUpdate()
{
    
    updateBounds();

    
    if(isGrounded)
    {   
        if(canRoll)
        {
            //check if there is a slope
            //roll in the direction of gravity//
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
              if(_bounds.IsColliding(_bounds, PhysicsObjects->at(i)->_bounds))
           // if(CheckCollisionBoxSphere(_bounds.GetBoundingBox() , PhysicsObjects->at(i)->parentObject->position, PhysicsObjects->at(i)->radius))
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
             if(_bounds.IsColliding(_bounds, PhysicsObjects->at(i)->_bounds))
           // if(CheckCollisionBoxes(_bounds.GetBoundingBox(), PhysicsObjects->at(i)->_bounds.GetBoundingBox()))
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
        //check side of collision and prevent movement//
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
            Vector3 diff = Vector3Subtract(other->parentObject->position, parentObject->position);
            float len = Vector3Length(diff);
            float velocityLen = Vector3Length(other->velocity);
            float impulseAmount = 1.5f;
            if (velocityLen > 0)
            {
                impulseAmount += velocityLen;
            }
            Vector3 impulse = Vector3Scale(diff, impulseAmount / len);
            impulse.y = 0;
            other->velocity = Vector3Add(other->velocity, Vector3Scale(impulse, 1 / other->mass));

                        
       }
    }
    for (size_t i = 0; i < collisionEvents.size(); i++)
    {
        
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
    debugLog("Added physics object");
        
}
