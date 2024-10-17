#ifndef ENTITY_H
#define ENTITY_H
#include <vector>
#include <raylib.h>
#include <unordered_map>
#define MAX_ENTITIES 100
///Data Cache: 32KB
//L2 cache: 256 KB
//L3 cache: 2 MB (shared between CPU and GPU)
struct EntityComponent
{

};
struct TransformComponent : public EntityComponent
{
    Vector3 position;
    float scale;
};

struct ModelComponent : public EntityComponent
{
    int modelID;
    float modelScale;
    Vector3 min;
    Vector3 max;
};
struct Entity
{
    int index;
};

namespace RenderSystem
{
    std::vector<Model> modelsLoaded;
    void Draw(std::vector<Entity>& entities, std::vector<TransformComponent>& transforms, std::vector<ModelComponent>& models)
    {
        for (auto& entity : entities)
        {
            // Get the transform and model components for this entity
            auto& transform = transforms[entity.index];
            auto& model = models[entity.index];

            // Render the entity using the transform and model components
            DrawModel(modelsLoaded.at(model.modelID), transform.position, model.modelScale, WHITE);
        }
    }
}
namespace EntityManager
{
  
    std::vector<Entity> entities;
    Model model;
    Entity& GetEntity(size_t index);
    size_t AddEntity();
    void RemoveEntity(size_t index);
    // Entity-Component Mapping
    std::unordered_map<Entity, std::vector<EntityComponent>> entityComponents;

    // Add a component to an entity
    void AddComponent(Entity entity, EntityComponent component)
    {
        auto& components = entityComponents[entity];
        components.push_back(component);
    }

    // Remove a component from an entity
    void RemoveComponent(Entity entity, EntityComponent component)
    {
        auto& components = entityComponents[entity];
        components.erase(std::remove(components.begin(), components.end(), component), components.end());
    }

    // Get the components for an entity
    std::vector<EntityComponent> GetComponents(Entity entity)
    {
        auto it = entityComponents.find(entity);
        if (it != entityComponents.end())
        {
            return it->second;
        }
        else
        {
            return {};
        }
    }


};

#endif // ENTITY_H
