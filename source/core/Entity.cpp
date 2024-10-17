#include <Entity.h>
#include <bits/algorithmfwd.h>

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

void EntityManager::DrawEntites()
{

    for (size_t i = 0; i < entities.size(); i++)
    {
        DrawModel(model, entities[i].position, entities[i].modelScale, WHITE);
    }
    
}

EntityManager::EntityManager()
{
    
}
