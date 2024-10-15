#include "GameObject.h"
#include "../debug/debug.h"


extern std::vector<GameObject *> *GameObjects;//NEED THIS FOR CALLBACKS
extern std::vector<EngineObject *> *GraphicsObjects;//NEED THIS FOR CALLBACKS

void GameObject::Draw()
{

}


void GameObject::AddChild(GameObject *child)
{
    children.push_back(child);
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
   
    delete objectModel;
   

}

