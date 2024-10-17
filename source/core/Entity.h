#ifndef ENTITY_H
#define ENTITY_H
#include <vector>
#include <raylib.h>
#define MAX_ENTITIES 100
///Data Cache: 32KB
//L2 cache: 256 KB
//L3 cache: 2 MB (shared between CPU and GPU)

struct Entity
{
    int index;
    //int modelID; //index to model in vector of models
    Vector3 position;
    float scale;
};
//vector of loaded models
class EntityManager
{
  public:
    std::vector<Entity> entities;
    Model model;
    Entity& GetEntity(size_t index);
    size_t AddEntity();
    void RemoveEntity(size_t index);
    void DrawEntites();
    void SetModel(Model model)
    {
        this->model = model;
    };
    EntityManager();


};

#endif // ENTITY_H
