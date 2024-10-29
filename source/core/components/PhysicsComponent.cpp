#include <PhysicsComponent.h>
#include <raymath.h>
#include "../debug/debug.h" 
#include <PhysicsWorld.h>
#include <RenderSystem.h>   
extern std::vector<PhysicsComponent *> *PhysicsObjects;
extern std::vector<Physics2DComponent *> *Physics2DObjects;
extern PhysicsWorld *physicsWorld;

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
        else
        {
            _bounds.boxColor = RED;
        }

    }


    
}

void PhysicsComponent::onCollision(PhysicsComponent *other)//update to get angular rotation
{

    if(!isKinematic)
    {
        if(other->isKinematic)
        {
            velocity = Vector3Add(velocity, this->_bounds.normal);
           _bounds.boxColor = BLUE;
        }
        else
        {

            parentObject->position = (Vector3Add(parentObject->position, this->_bounds.normal));//scale new pos with impulse based on velocity and mass
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

//BULLET
BulletPhysicsComponent::BulletPhysicsComponent(Vector3 pos, Quaternion rot, float mass, btCollisionShape *shape)
 {
    transform = new btTransform(btQuaternion(btScalar(rot.z), btScalar(rot.y), btScalar(rot.x)));
    transform->setOrigin(btVector3(pos.x, pos.y, pos.z));
    this->mass = btScalar(mass);
    this->localInertia = btVector3(0, 0, 0);
    this->myMotionState = new btDefaultMotionState(*transform);
    collisionShapes.push_back(shape);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, shape, localInertia);
    btRigidBody* body = new btRigidBody(rbInfo);
    body->setWorldTransform(*transform);
    physicsWorld->dynamicsWorld->addRigidBody(body);
}
//Handle Bullet Collisions
void BulletPhysicsComponent::onCollision(PhysicsComponent *other)
{

}
void BulletPhysicsComponent::onTrigger(PhysicsComponent *other) 
{

}

void BulletPhysicsComponent::OnUpdate()
{
    if(!parentObject->isActive || !isActive)
    {
        return;
    }
    BindMatrix();
}

//2D Physics
//Handle 2D Collisions
void Physics2DComponent::onCollision(PhysicsComponent *other)
{

}

void Physics2DComponent::onTrigger(PhysicsComponent *other)
{

}

void Physics2DComponent::ComponentAddedCallback()
{

}

void Physics2DComponent::OnUpdate()
{

}
