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
    _bounds.UpdateCorners(parentObject->position, size);
    Matrix rotMatrix = MatrixRotateXYZ({parentObject->rotation.x, parentObject->rotation.y, parentObject->rotation.z});
    _bounds.Rotate(&rotMatrix);
}
void PhysicsComponent::SetAngularVelocity(float vel, Vector3 axis)
{
    velocity = Vector3Add(velocity, Vector3Scale(axis, vel));

}
void PhysicsComponent::OnUpdate()
{
    
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
            if(_bounds.CheckSATCollision(_bounds, PhysicsObjects->at(i)->_bounds))
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
            if(_bounds.CheckSATCollision(_bounds, PhysicsObjects->at(i)->_bounds))
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
        Vector3 dir = Vector3Subtract(parentObject->position, other->parentObject->position);
        if(other->isKinematic)
        {
                Ray ray = {parentObject->position, dir};
                RayCollision collision = _bounds.GetSATCollision(_bounds, other->_bounds, ray);
                //RayCollision collision = PhysicsWorld::ShootRayCollision(parentObject->position, dir, other->_bounds.GetBoundingBox());
                if(parentObject->position.y < other->parentObject->position.y)//standing on top of an object
                {
                    parentObject->position.y = collision.point.y;
                    velocity.y = 0;
                    groundedYPos = parentObject->position.y;
                    isGrounded = true;
                }
                else if(parentObject->position.y > other->parentObject->position.y)//standing below an object
                {
                    velocity.y = 0;//no velocity let gravity take over//
                }
                //side faces
                else
                {
                    parentObject->position = collision.point;
                    velocity = Vector3Zero();
                }
                //get dot product of hit point and normal
                Vector3 currentDir = Vector3Subtract(parentObject->position, parentObject->forward);
                float dot = Vector3DotProduct(collision.normal, Vector3Normalize(velocity));
                debugLog("Dot Product: %f", dot);   
                _bounds.boxColor = BLUE;
        }
        else
        {
           
            float dist = Vector3Distance(parentObject->position, other->parentObject->position);
            RayCollision collision = PhysicsWorld::ShootRayCollision(parentObject->position, dir, other->_bounds.GetBoundingBox());
           // RayCollision collision = _bounds.GetSATCollision(_bounds,other->_bounds);
            Vector3 hitDir = Vector3Subtract(parentObject->position, collision.point);
            Vector3 oldPos = parentObject->position;
            if(dist > collision.distance)
            {
                dist = collision.distance;
            }
            Vector3 newPos =Vector3Lerp(oldPos, Vector3Subtract(parentObject->position, Vector3Scale(hitDir, dist)), 6.0f * GetFrameTime());//works
            parentObject->position = newPos;

            velocity = Vector3Add(velocity, Vector3Scale(collision.normal,-10.0f * GetFrameTime()));
            _bounds.boxColor = GREEN;
        }
        //Angular stuff

    }
    else
    {
        _bounds.boxColor = BLUE;
    }

    
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
