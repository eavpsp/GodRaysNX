#include <Entity.h>
#include <bits/algorithmfwd.h>
#include<arm_neon.h>
#include <ResourceManager.h>
#include "../debug/debug.h" 
std::vector<Model> modelsLoaded;

Entity& EntityManager::GetEntity(size_t index)
{
    return entities[index];
}

size_t EntityManager::AddEntity() 
{
    Entity newEntity;
    newEntity.index = entities.size();
    entities.push_back(newEntity);
    return entities.size() - 1;
}

void EntityManager::RemoveEntity(size_t index)
{
    entities.erase(entities.begin() + index);
}

void RenderSystemESC::AddComponent(Entity entity, ModelComponent component)
{
    entityWithModelComponents[entity] = component;
    debugLog("Model Component added");
}

void RenderSystemESC::RemoveComponent(Entity entity)
{
   entityWithModelComponents.erase(entity);
}

void RenderSystemESC::Draw(Model model)//4d6b0 offset 0x80758000
{

        for (const auto& [Entity, components] : entityWithModelComponents) 
        {
            TransformComponent transform = TransformSystemESC::GetComponent(Entity);

            DrawModel(model, transform.position, transform.scale, WHITE);//0x807a56b0
        }
}

void RenderSystemESC::Init()
{
    //load models
    
    //model = LoadModel("romfs:/models/robot.glb");

}

ModelComponent RenderSystemESC::GetComponents(Entity entity)
{
    return entityWithModelComponents[entity];
}

std::vector<Entity> RenderSystemESC::GetEntities()
{
    std::vector<Entity> entities;
    for (const auto& [Entity, components] : entityWithModelComponents)
    {
        entities.push_back(Entity);
    }
     
     return entities;
}

void EntityComponentSystem::UpdateSystem()
{
    
}
void EntityComponentSystem::InitSystem() 
{
    RenderSystemESC::Init();
    debugLog("Entity Component System Init");
};

void TransformSystemESC::UpdateTransform(Entity entity, Vector3 position, float scale)
{
    entityWithTransformComponents[entity].position = position;
    entityWithTransformComponents[entity].scale = scale;
}

void TransformSystemESC::AddComponent(Entity entity, TransformComponent component)
{
    entityWithTransformComponents[entity] = component;
    debugLog("Transform Component added");
}

void TransformSystemESC::RemoveComponent(Entity entity)
{
    entityWithTransformComponents.erase(entity);
}

TransformComponent TransformSystemESC::GetComponent(Entity entity)
{
    
    return entityWithTransformComponents[entity];
}

std::vector<Entity> TransformSystemESC::GetEntities()
{
   std::vector<Entity> entities;
    for (const auto& [Entity, components] : entityWithTransformComponents)
    {
        entities.push_back(Entity);
    }
    return entities;
}
