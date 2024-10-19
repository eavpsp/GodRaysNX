#include <PhysicsComponent.h>
#include <raymath.h>
#include "../debug/debug.h" 
#include <PhysicsWorld.h>
#include <RenderSystem.h>   
extern std::vector<PhysicsComponent *> *PhysicsObjects;
void PhysicsComponent::updateBounds()//
{
    
    _bounds.UpdateCorners(parentObject->position, size);
    Matrix rotMat = MatrixRotateXYZ(QuaternionToEuler (parentObject->rotation));
    _bounds.Rotate(&rotMat);
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
       
    }

    
}

void PhysicsComponent::onCollision(PhysicsComponent *other)
{
    
    Vector3 diff = Vector3Subtract(other->parentObject->position, parentObject->position);
   
    if(!isKinematic)
    {
      
       if(other->isKinematic)
       {
            float oldLeft = parentObject->position.x - _bounds.GetWidth();
            float oldRight = parentObject->position.x + _bounds.GetWidth();
            float oldFront = parentObject->position.z + _bounds.GetDepth();
            float oldBack = parentObject->position.z - _bounds.GetDepth();
            bool canHitX = true;
            bool canHitZ = true;
            
            if (velocity.x + parentObject->position.x > other->parentObject->position.x + other->_bounds.GetWidth())	// our front is to the right of wall back
                canHitX = false;
            else if (velocity.x + parentObject->position.x - _bounds.GetWidth() < other->parentObject->position.x) // our back is to the left of the wall front
                canHitX = false;
            
            if (velocity.y + parentObject->position.y > other->parentObject->position.y + other->_bounds.GetHeight())	// our top is above wall bottom
                canHitX = false;
            else if (velocity.y + parentObject->position.y - _bounds.GetHeight() < other->parentObject->position.y) // our bottom is below the wall top
                canHitX = false;
            
            if (velocity.z + parentObject->position.z > other->parentObject->position.z + other->_bounds.GetDepth())	// our front is below wall back
                canHitZ = false;
            else if (velocity.z + parentObject->position.z - _bounds.GetDepth() < other->parentObject->position.z) // our back is over the wall front
                canHitZ = false;
            
            if (canHitX)
            {
                float newLeft = velocity.x + parentObject->position.x - _bounds.GetWidth();
                float newRight = velocity.x + parentObject->position.x + _bounds.GetWidth();
                float objectLeft = other->parentObject->position.x - other->_bounds.GetWidth();
                float objectRight = other->parentObject->position.x + other->_bounds.GetWidth();

                // check the box moving to the left
                // if we were outside the right wall before, and are not now, we hit something
                if (velocity.x < 0)
                {
                    if (oldRight > other->parentObject->position.x)
                    {
                        if (newLeft < other->parentObject->position.x)
                        {
                            // we hit moving to the left, so set us back to where we hit the wall
                            parentObject->position.x = other->parentObject->position.x + other->_bounds.GetWidth();
                            //if (hitSide)
                            //    *hitSide = true;
                        }
                    }		
                }
                
                if (velocity.x > 0)
                {
                    // check the box moving to the right
                    // if we were outside the left wall before, and are not now, we hit something
                    if (velocity.x >= objectRight)
                    {
                        if (velocity.x + parentObject->position.x > objectRight)
                        {
                            // we hit moving to the right, so set us back to where we hit the wall
                            parentObject->position.x = objectRight;
                            //if (hitSide)
                            //    *hitSide = true;
                        }
                    }
                }
            }
            
            if (canHitZ)
            {
                float newFront = velocity.z + parentObject->position.z + _bounds.GetDepth();
                float objectFront = other->parentObject->position.z + other->_bounds.GetDepth();

                // check the box moving to the front
                // if we were outside the back wall before, and are not now, we hit something
                if (velocity.z > 0)
                {
                    if (oldFront <= other->parentObject->position.z)
                    {
                        if (newFront > other->parentObject->position.z)
                        {
                            // we hit moving to the front, so set us back to where we hit the wall
                            parentObject->position.z = parentObject->position.z - _bounds.GetDepth();
                            //if (hitSide)
                            //    *hitSide = true;
                        }
                    }		
                }
                
                if (velocity.z < 0)
                {
                    // check the box moving to the back
                    // if we were outside the front wall before, and are not now, we hit something
                    if (velocity.z >= objectFront)
                    {
                        if (velocity.z + parentObject->position.z < objectFront)
                        {
                            // we hit moving to the back, so set us back to where we hit the wall
                            parentObject->position.z = objectFront;
                            //if (hitSide)
                            //    *hitSide = true;
                        }
                    }
                }
            }
       }
       else
       {
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
 
        //is Kinematic
        //prevent object
    
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
