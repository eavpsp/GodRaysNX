#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <EngineObject.h>
#include <raymath.h>
/**
 * @class GameComponent
 * @brief This is the base class for all game components.
 * @details This class is the base class for all game components. It provides a common interface for all components to inherit from and implement. The interface includes a method to update the component and a method to set the parent object of the component.
 */

struct GameComponent
{
    public:
        EngineObject* parentObject;
        virtual void OnUpdate() = 0;
        void SetParentObject(EngineObject* obj) { parentObject = obj; }
        bool isActive = true;

        virtual void ComponentAddedCallback() = 0;
        GameComponent(){};
        virtual ~GameComponent() {}
};
struct ObjectDrawable
{
    GameComponent* componentParent;
    virtual void draw(){};
};

/**
 * @file GameObject.h
 * @brief This class is an extension of the EngineObject class. It adds additional functionality that is specific to objects in the game world.
 * @details GameObjects can have children, a parent, and they can draw their own models. This class is used to create objects that are a bit more complex than the standard EngineObject.
 * used as a template for all objects to inherit from. Registers to script callbacks to manage update ticks and drawing calls
 */
struct GameObject : public EngineObject
{
    private:
        GameObject *parent = nullptr;
        std::vector<GameObject*> children;
        void onInit() override;
        void onDestroy() override;
    public:
        //base model data
        bool isStatic;
        int gameObjectID;
        void Draw();

        template <typename T>
        static T* InstantiateGameObject(Vector3 _position, Quaternion _rotation, float _scale, const char* _name = "GameObject")
        {
            if (!std::is_base_of<EngineObject, T>::value) {
                // Error: T is not a derived class of EngineObject
                return nullptr;
            }
                T *newObject =  new T();
                //add callback
                //newObject->material = mat;
                newObject->transform = Matrix();
                newObject->position = Vector3{_position.x, _position.y, _position.z};
                newObject->rotation = Quaternion{_rotation.x, _rotation.y, _rotation.z, _rotation.w};
                newObject->scale = _scale;
                newObject->name = _name;
                newObject->onInit();
                return newObject;
        }
        void AddChild(GameObject *child);
        GameObject *GetChild(int index);
        int GetChildrenCount();
        void SetParent(GameObject *i_parent);
        GameObject *GetParent();
        void RegisterObject();
        void onUpdate() override;
        void onDraw() override;
        std::vector<GameComponent *> components;
        void AddComponent(GameComponent *component)
        {
            components.push_back(component);
            component->SetParentObject(this);
            component->ComponentAddedCallback();
        };
        void UpdateComponents()
        {
            for (int i = 0; i < components.size(); i++)
            {
                components.at(i)->OnUpdate();
            }
        }
        
        void RemoveComponent(GameComponent *component)
        {
            for (int i = 0; i < components.size(); i++)
            {
                if (components[i] == component)
                {
                    components.erase(components.begin() + i);
                    break;
                }
            }
        };
        template<typename T>
        T *GetComponent()
        {
            for (int i = 0; i < components.size(); i++)
            {
                if (dynamic_cast<T*>(components[i]))
                {
                    return dynamic_cast<T*>(components[i]);
                }
            }
            return nullptr;
        }
        
        GameObject();
        virtual ~GameObject();
        //Draw Primatives
      
    };

 




#endif // GAMEOBJECT_H
