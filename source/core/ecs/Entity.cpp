#include <Entity.h>
#include <bits/algorithmfwd.h>
#include<arm_neon.h>
#include <ResourceManager.h>
#include "../debug/debug.h" 
RenderSystemECS renderSystemECS;
TransformSystemECS transformSystem;

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

void RenderSystemECS::AddComponent(Entity entity, ModelComponent component)
{
    entityWithModelComponents[entity] = component;
    
}

void RenderSystemECS::RemoveComponent(Entity entity)
{
   entityWithModelComponents.erase(entity);
}


void RenderSystemECS::Draw(Model model, Vector3 position, float scale)//make one to send in a list
{

       
    DrawModel(model, position, scale, WHITE);
        
}

void RenderSystemECS::DrawAll(Model model)
{

      for (const auto& [Entity, components] : entityWithModelComponents) 
        {
            
            TransformComponent transform = transformSystem.GetComponent(Entity);

            Draw(model, transform.position, transform.scale);
        }
}

void RenderSystemECS::Init()
{


}

void RenderSystemECS::DrawEntities(Model model, Entity entity)//needs camera
{
   
        TransformComponent transform = transformSystem.GetComponent(entity);
        Draw(model, transform.position, transform.scale);
    
}

ModelComponent RenderSystemECS::GetComponents(Entity entity)
{
    return entityWithModelComponents[entity];
}

std::vector<Entity> RenderSystemECS::GetEntities()
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
    transformSystem.Init();
    renderSystemECS.Init();
    debugLog("Entity Component System Init");
};

void TransformSystemECS::Init()
{

}

void TransformSystemECS::UpdateTransform(Entity entity, Vector3 position, float scale)
{
    entityWithTransformComponents[entity].position = position;
    entityWithTransformComponents[entity].scale = scale;
}

void TransformSystemECS::AddComponent(Entity entity, TransformComponent component)
{
    entityWithTransformComponents[entity] = component;
  
}

void TransformSystemECS::RemoveComponent(Entity entity)
{
    entityWithTransformComponents.erase(entity);
}

TransformComponent TransformSystemECS::GetComponent(Entity entity)
{
    
    return entityWithTransformComponents[entity];
}

std::vector<Entity> TransformSystemECS::GetEntities()
{
   std::vector<Entity> entities;
    for (const auto& [Entity, components] : entityWithTransformComponents)
    {
        entities.push_back(Entity);
    }
    return entities;
}
