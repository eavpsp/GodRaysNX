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

// BULLET

void BulletPhysicsComponent::DrawCollider()
{
     if (shape->getShapeType() == 8)
    {

            btSphereShape *sphere = (btSphereShape *)shape;
            DrawSphereWires(parentObject->position, shape->getMargin(), 3, 3, GREEN);
    }
    else if(shape->getShapeType() == 13)
    {
            btCylinderShape *cyl = (btCylinderShape *)shape;
            DrawCylinderWires(parentObject->position, cyl->getHalfExtentsWithMargin().getX(), cyl->getHalfExtentsWithMargin().getY(), cyl->getHalfExtentsWithMargin().getZ(), 32, GREEN);
    }
    else if(shape->getShapeType() == 10)
    {
            btCapsuleShape *capsule = (btCapsuleShape *)shape;
            DrawCapsuleWires(parentObject->position,  Vector3Add(parentObject->position, parentObject->upVector), capsule->getRadius(), 32, 32, GREEN);
    }
     else if(shape->getShapeType() == 0)
     {
            btBoxShape *box = (btBoxShape *)shape;
            DrawCubeWires(parentObject->position, box->getHalfExtentsWithMargin().getX(), box->getHalfExtentsWithMargin().getY(), box->getHalfExtentsWithMargin().getZ(), GREEN);
    }
}

BulletPhysicsComponent::BulletPhysicsComponent(Vector3 pos, Quaternion rot, float mass, btCollisionShape *shape)
{
    transform = new btTransform(btQuaternion(btScalar(rot.z), btScalar(rot.y), btScalar(rot.x)));
    transform->setOrigin(btVector3(pos.x, pos.y, pos.z));
    this->mass = btScalar(mass);
    this->localInertia = btVector3(0, 0, 0);
    this->myMotionState = new btDefaultMotionState(*transform);
    this->shape = shape;
    this->collisionObject = new btCollisionObject();
    this->collisionObject->setCollisionShape(shape);
    this->collisionObject->setUserPointer(this);
    this->collisionObject->setUserIndex(0);
    this->collisionObject->setWorldTransform(*transform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(this->mass, myMotionState, this->shape, localInertia);
    this->body = new btRigidBody(rbInfo);
    this->body->setWorldTransform(*transform);
    this->body->setUserPointer(this);
    physicsWorld->dynamicsWorld->addRigidBody(this->body);
    physicsWorld->collisionObjectMap[this->body] = this;
}
//Handle Bullet Collisions
extern std::map<const btCollisionObject*, CollisionInfo*> objectsCollisionInfoMap;
extern std::map<const btCollisionObject*,std::vector<btManifoldPoint*>> objectsCollisions;
bool checkCollision(btCollisionObject* other)
{
        btRigidBody *body = btRigidBody::upcast(other);
        btTransform trans;
        if (body && body->getMotionState()) {
            body->getMotionState()->getWorldTransform(trans);
        } else {
            trans = other->getWorldTransform();
        }
        auto& manifoldPoints = objectsCollisions[body];
        if (manifoldPoints.empty()) 
        {
            return false;
        } 
        else 
        {
         
          return true;
                
        }
  
}
const btCollisionObject* getCollidedBody(btCollisionObject* other) 
{
    if(objectsCollisionInfoMap[other]->bodyB != nullptr)
    {
        return objectsCollisionInfoMap[other]->bodyB;

    }
    return nullptr;
}
void BulletPhysicsComponent::onCollision(BulletPhysicsComponent *other)
{
    if(other == nullptr)
    {
        return;
    }   
    for (size_t i = 0; i < collisionEvents.size(); i++)
    {
        
        collisionEvents[i]->DoEvent(other);
    }
    
}
void BulletPhysicsComponent::onTrigger(BulletPhysicsComponent *other)
{
    if(other == nullptr)
    {
        return;
    }  
    for (size_t i = 0; i < triggerEvents.size(); i++)
    {
        
        triggerEvents[i]->DoEvent(other);
    }
}
void BulletPhysicsComponent::onCollisionExit(BulletPhysicsComponent *other)
{
    if(other == nullptr)
    {
        return;
    }  
    for (size_t i = 0; i < collisionExitEvents.size(); i++)
    {
        collisionExitEvents[i]->DoEvent(other);
    }
    debugLog("Collision Exit");
    debugLog("Collision Exit: %s", other->parentObject->name.c_str());
   
    
}
void BulletPhysicsComponent::onTriggerExit(BulletPhysicsComponent * other)
{
    if(other == nullptr)
    {
        return;
    }  
    for (size_t i = 0; i < triggerExitEvents.size(); i++)
    {
        
        triggerExitEvents[i]->DoEvent(other);
    }
    
}
std::vector<BulletPhysicsComponent*> currentCollidingObjects;
std::vector<BulletPhysicsComponent*> prevCollidingObjects;
void BulletPhysicsComponent::OnUpdate()
{
    if(!parentObject->isActive || !isActive)
    {
        return;
    }
    BindMatrix();
    //check for collisions
    currentCollidingObjects.clear();
    for (int j = physicsWorld->dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
    {
        if(checkCollision(physicsWorld->dynamicsWorld->getCollisionObjectArray()[j]))
        {
            const btCollisionObject* body = getCollidedBody(physicsWorld->dynamicsWorld->getCollisionObjectArray()[j]);
            BulletPhysicsComponent* otherObj = static_cast<BulletPhysicsComponent*>(body->getUserPointer());
            currentCollidingObjects.push_back(otherObj);
            if(!isKinematic)
            {
                if(isTrigger)
                {
                    onTrigger(otherObj);
                }
                else
                {
                    onCollision(otherObj);
                }
            }

        }
       
       
    }

    for (auto it = prevCollidingObjects.begin(); it != prevCollidingObjects.end(); ++it)
    {
        if (std::find(currentCollidingObjects.begin(), currentCollidingObjects.end(), *it) == currentCollidingObjects.end()) 
        {
            if(!isKinematic)
            {
                if((*it)->isTrigger)
                {
                    onTriggerExit(*it);
                }
                else
                {
                    onCollisionExit(*it);
                }
            }
            
        }
    }
    
    
    prevCollidingObjects = currentCollidingObjects;

    
}

//2D Physics
//Handle 2D Collisions
void Physics2DComponent::onCollision(Physics2DComponent *other)
{

}

void Physics2DComponent::onTrigger(Physics2DComponent *other)
{

}

void Physics2DComponent::ComponentAddedCallback()
/**
 * @brief Called when the component is added to a GameObject.
 * @details This is called when the component is added to a GameObject. It is used to initialize the component and add it to the list of physics components.
 */
{
    Physics2DObjects->push_back(this);//add to list of physics Objects
}

void Physics2DComponent::OnUpdate()
{
    
    rect.x = parentObject->position.x;
    rect.y = parentObject->position.y;
    //check for collisions
    for (size_t i = 0; i < Physics2DObjects->size(); i++)
    {
        if(shape == CollisionShape::BOX)
        {
            if(Physics2DObjects->at(i)->shape == CollisionShape::BOX)
            {
                if(CheckCollisionRecs(rect, Physics2DObjects->at(i)->rect))
                {
                    if(Physics2DObjects->at(i)->isTrigger)
                    {
                        onTrigger(Physics2DObjects->at(i));

                    }
                    else
                    {
                        onCollision(Physics2DObjects->at(i));

                    }
                }
            }
            else
            {
                if(CheckCollisionCircleRec(Vector2{Physics2DObjects->at(i)->parentObject->position.x, Physics2DObjects->at(i)->parentObject->position.y}, Physics2DObjects->at(i)->radius, Physics2DObjects->at(i)->rect))
                {
                     if(Physics2DObjects->at(i)->isTrigger)
                    {
                        onTrigger(Physics2DObjects->at(i));

                    }
                    else
                    {
                        onCollision(Physics2DObjects->at(i));

                    }
                }
            }
           
        }
        else
        {
             if(Physics2DObjects->at(i)->shape == CollisionShape::BOX)
            {
                if(CheckCollisionCircleRec(Vector2{Physics2DObjects->at(i)->parentObject->position.x, Physics2DObjects->at(i)->parentObject->position.y}, radius, Physics2DObjects->at(i)->rect))
                {
                    if(Physics2DObjects->at(i)->isTrigger)
                    {
                        onTrigger(Physics2DObjects->at(i));

                    }
                    else
                    {
                        onCollision(Physics2DObjects->at(i));

                    }
                }
            }
            else
            {
                if(CheckCollisionCircles(Vector2{Physics2DObjects->at(i)->parentObject->position.x, Physics2DObjects->at(i)->parentObject->position.y}, Physics2DObjects->at(i)->radius, Vector2{parentObject->position.x, parentObject->position.y}, radius))
                {
                     if(Physics2DObjects->at(i)->isTrigger)
                    {
                        onTrigger(Physics2DObjects->at(i));

                    }
                    else
                    {
                        onCollision(Physics2DObjects->at(i));

                    }
                }
            }
        }
        
    }
    

}

Physics2DComponent::Physics2DComponent(float mass, float radius, bool _isTrig, bool _isKinematic)
{
    this->mass = mass;
    this->radius = radius;
    this->isTrigger = _isTrig;
    this->isKinematic = _isKinematic;
}

Physics2DComponent::Physics2DComponent(float mass, Vector2 _size, bool _isTrig, bool _isKinematic)
{
    this->mass = mass;
    this->isTrigger = _isTrig;
    this->isKinematic = _isKinematic;
    this->rect = {parentObject->position.x, parentObject->position.y, _size.x, _size.y};
}
