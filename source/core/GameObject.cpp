#include "GameObject.h"
#include "../debug/debug.h"
#include <AnimationController.h>

extern std::vector<GameObject *> *GameObjects;//NEED THIS FOR CALLBACKS
extern std::vector<EngineObject *> *GraphicsObjects;//NEED THIS FOR CALLBACKS

void GameObject::Draw()
{
    if(!isActive)
    {
        return;
    }
    if(objectDrawables.size() != 0)
    {
       for (size_t i = 0; i < objectDrawables.size(); i++)
       {
           objectDrawables.at(i)->draw();
       }
       

    }
    onDraw();
  
}


void GameObject::AddChild(GameObject *child)
{
    children.push_back(child);
    child->SetParent(this);
}
GameObject *GameObject::GetChild(int index)
{
    return children.at(index);
}

int GameObject::GetChildrenCount()
{
    return children.size();
}

void GameObject::SetParent(GameObject *i_parent)
{
    parent = i_parent;
}

GameObject *GameObject::GetParent()
{
    return parent;
}

void GameObject::RegisterObject()
{
    
    GameObjects->push_back(this);
}

void GameObject::onUpdate()
{
    if(!isActive)
    {
        return;
    }
    if(parent != nullptr)//check if there is a parent object to follow
    {
        position = Vector3Add(parent->position, localPosition);
        rotation = QuaternionAdd(parent->rotation, localRotation);
    }
    transform = MatrixMultiply(MatrixTranslate(position.x, position.y, position.z), MatrixMultiply(QuaternionToMatrix(rotation), MatrixScale(scale, scale, scale)));
    forward = Vector3Transform({0, 0, 1}, transform);
    AnimationComponent* animComp = GetComponent<AnimationComponent>();
    if(animComp != nullptr)
    {
        animComp->animationController->OnUpdate();
    }
   UpdateComponents();//NEDED TO UPDATE COMPONENT DATA
    
}

void GameObject::onDraw()
{

}

void GameObject::onInit()
{

}

void GameObject::onDestroy()
{

}

GameObject::GameObject()
{
    RegisterObject();//NEDED FOR CALLBACKS
    onInit();
    
};
 GameObject::~GameObject()
{
   
    onDestroy();
    for (size_t i = 0; i < objectDrawables.size(); i++)
    {   
        delete objectDrawables.at(i);
    }
    
    //destroy children
    for (int i = 0; i < children.size(); i++)
    {
        delete children[i];
    }
    //destroy components
    for (int i = 0; i < components.size(); i++)
    {
        delete components[i];
    }

   for (size_t i = 0; i < GameObjects->size(); i++)
   {
    /* code */
        if(this == GameObjects->at(i))
        {
            GameObjects->erase(GameObjects->begin() + i);
            break;
        }
   }
   delete this;

}

