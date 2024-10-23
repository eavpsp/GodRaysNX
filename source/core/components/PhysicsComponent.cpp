#include <PhysicsComponent.h>
#include <raymath.h>
#include "../debug/debug.h" 
#include <PhysicsWorld.h>
#include <RenderSystem.h>   
extern std::vector<PhysicsComponent *> *PhysicsObjects;
/*

First, calculate the projection of the gravity vector onto the plane normal:

g_proj = g * (n * g) / (n * n)

where:

g is the gravity vector (e.g. [0, -9.8, 0])
n is the plane normal vector ([-0.000000, 25.881195, 96.592712])
Then, calculate the component of the gravity vector that is perpendicular to the plane:

g_perp = g - g_proj
If the object doesn't have a radius, you can't use the formula ω = v / r to calculate the angular velocity.

However, you can still calculate the angular velocity using the plane normal and the gravity vector.

One way to do this is to use the following formula:

ω = g_perp / (m * sin(θ))

where:

g_perp is the component of the gravity vector that is perpendicular to the plane
m is the mass of the object
θ is the angle between the plane normal and the gravity vector
This formula is derived from the torque equation for an object on an inclined plane, and it doesn't require the object to have a radius.

Alternatively, you can use the following formula:

ω = √(g_perp / (m * (n * g)))

where:

g_perp is the component of the gravity vector that is perpendicular to the plane
m is the mass of the object
n is the plane normal vector
g is the gravity vector

*/

void PhysicsComponent::updateBounds()//
{
    _bounds.UpdateCollider(parentObject->position);
    _bounds.UpdateCorners(parentObject->position, size);
    _bounds.Rotate(&parentObject->rotation);
}
void PhysicsComponent::SetAngularVelocity(float vel, Vector3 axis)
{
    velocity = Vector3Add(velocity, Vector3Scale(axis, vel));

}
void PhysicsComponent::OnUpdate()
{
    
    parentObject->forward = Vector3Normalize(Vector3Add(parentObject->position, velocity));
    updateBounds();
    if(parentObject->position.y != groundedYPos)
    {
        isGrounded = false;
    }
    //update this to use an octree//
    for (size_t i = 0; i < PhysicsObjects->size(); i++)
    {
        if(PhysicsObjects->at(i) == this)
        {
            continue;
        }
        if(PhysicsObjects->at(i)->shape == SPHERE)
        {
             if(_bounds.CheckCollision(this->_bounds.collider, PhysicsObjects->at(i)->_bounds.collider, &this->_bounds.normal))
              //if(CheckCollisionBoxSphere(_bounds.GetBoundingBox(),PhysicsObjects->at(i)->parentObject->position, PhysicsObjects->at(i)->radius)) 
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
             else{
                 _bounds.boxColor = RED;

            }
        }
        else
        {
            if(_bounds.CheckCollision(this->_bounds.collider, PhysicsObjects->at(i)->_bounds.collider, &this->_bounds.normal))
            // if(CheckCollisionBoxes(_bounds.GetBoundingBox(), PhysicsObjects->at(i)->_bounds.GetBoundingBox())) 
            {
                 //
               
                if(isTrigger)
                {
                        onTrigger(PhysicsObjects->at(i));
                }
                else
                {
                        onCollision(PhysicsObjects->at(i));
                }
            }
            else
            {
                 _bounds.boxColor = RED;

            }
           
        }
       
    }


    
}

void PhysicsComponent::onCollision(PhysicsComponent *other)//update to get angular rotation
{

    if(!isKinematic)
    {
        if(other->isKinematic)
        {
            parentObject->position = Vector3Add(parentObject->position, this->_bounds.normal);
           _bounds.boxColor = BLUE;
        }
        else
        {
        
            parentObject->position = Vector3Add(parentObject->position, this->_bounds.normal);
            _bounds.boxColor = GREEN;

        }
        
    }
  

    
  
  //EVENTS
   {
        for (size_t i = 0; i < collisionEvents.size(); i++)
        {
            
            collisionEvents[i]->DoEvent(other);
        }
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
