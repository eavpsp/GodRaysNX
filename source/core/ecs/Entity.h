#ifndef ENTITY_H
#define ENTITY_H
#pragma once
#include <vector>
#include <raylib.h>
#include <map>
#include<typeinfo>
#include <map>
#include <string>
#include <list>
#include <memory>
#include<array> 
#include <typeindex>
#include <algorithm>
#include <any>
#define MAX_ENTITIES 100
///Data Cache: 32KB
//L2 cache: 256 KB
//L3 cache: 2 MB (shared between CPU and GPU)
struct __attribute__((packed)) TransformComponent
{
    Vector3 position;
    float scale;

};
struct __attribute__((packed)) ModelComponent
//must have a transform component
{
    int modelIndex;
    float modelScale;
    Vector3 min;
    Vector3 max;
    
};
struct __attribute__((packed)) Entity
{
    uint32_t index;
    Entity() : index(-1) {};
    bool operator<(const Entity& rhs) const 
    {
        return index < rhs.index;
    }   
};

struct TransformSystemECS
{
public:
    void Init();
    void UpdateTransform(Entity entity, Vector3 position, float scale);
    std::map <Entity, TransformComponent> entityWithTransformComponents;
    void AddComponent(Entity entity, TransformComponent component);
    void RemoveComponent(Entity entity);
    TransformComponent GetComponent(Entity entity);
    std::vector<Entity> GetEntities();
};

struct RenderSystemECS
{
public:
    void Init();
    void DrawEntities(Model model, Entity entity);
    void Draw(Model model, Vector3 position, float scale);
    void DrawAll(Model model);
    std::map <Entity, ModelComponent> entityWithModelComponents;
    void AddComponent(Entity entity, ModelComponent component);
    void RemoveComponent(Entity entity);
    ModelComponent GetComponents(Entity entity);
    std::vector<Entity> GetEntities();
};
namespace EntityManager
{
  
    static std::vector<Entity> entities;
    Entity& GetEntity(size_t index);
    size_t AddEntity();
    void RemoveEntity(size_t index);
    
};

struct EntityComponentSystem
{
    public:

    //Run all systems here
    void UpdateSystem();
    
    void InitSystem();

}; 
#endif // ENTITY_H
